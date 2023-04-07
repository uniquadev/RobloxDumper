#include "JobsHandler.h"
#include "Jobs.h"
#include "Memory.h"

#include <synchapi.h>

void Dumper::JobsHandler::register_jobs()
{
	// INDIPENDENTS JOBS
	register_job("Misc", dump_misc);
	register_job("Taskscheduler", dump_taskscheduler);
	register_job("LTypeName", dump_ltypename);
	register_job("SCResume", dump_scresume);
	register_job("lua_openbase", dump_lua_openbase);
	register_job("TaskDefer", dump_taskdefer);

	// DEPENDS ON SCRIPTMODULEINTERNAL & VMLOAD
	register_job("LuaVM::Load", dump_vm_load, {
		{"pseudo2addr", dump_pseudo2addr},
		{"vmload_hooks", dump_vmload_hooks}
	});

	// ADDONS
	// TODO subs calling convetions dumper
	// TODO subs retcheck dumper
	// TODO check if luau fast region
}

CRITICAL_SECTION init_section;

void Dumper::JobsHandler::run_job(Dumper::Job job)
{
	if (!job.f(this))	// if job return false then force stop dumping
		push_error("ERROR AT " + job.name);
	else
		for (const auto& child : job.children)
			run_job(child);
}

bool Dumper::JobsHandler::run()
{
	std::ignore = InitializeCriticalSectionAndSpinCount(&init_section, 0x00000200UL);
	std::clock_t started_at = std::clock();
	
	EnterCriticalSection(&init_section);
	for (const auto& job : jobs)
		run_job(job);
	LeaveCriticalSection(&init_section);

	std::stringstream stream2;
	stream2 << 1000.0 * (std::clock() - started_at) / CLOCKS_PER_SEC << " ms";
	output["cputime"] = stream2.str();

	return errors == 0 ? true : false;
}

void Dumper::JobsHandler::format()
{
	const auto path = Dumper::Memory::get_dll_dir((HMODULE)Dumper::Memory::r_module);
	output["version"] = path.filename().string();
}