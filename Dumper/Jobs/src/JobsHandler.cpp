#include "JobsHandler.h"
#include "Jobs.h"
#include "Memory.h"

#include <synchapi.h>

void Dumper::JobsHandler::register_jobs()
{
	jobs.push_back({ "Misc", dump_misc });
	jobs.push_back({ "Taskscheduler", dump_taskscheduler });
	jobs.push_back({ "LTypeName", dump_ltypename });
	jobs.push_back({ "LuaVM::Load", dump_vm_load });
}

CRITICAL_SECTION init_section;

bool Dumper::JobsHandler::run()
{
	std::ignore = InitializeCriticalSectionAndSpinCount(&init_section, 0x00000200UL);
	std::clock_t started_at = std::clock();
	
	EnterCriticalSection(&init_section);
	for (const auto& job : jobs)
	{
		if (!job.f(this))	// if job return false then force stop dumping
		{
			LeaveCriticalSection(&init_section);
			push_error("STOPPED AT " + job.name);
			return false;
		}
	}
	LeaveCriticalSection(&init_section);

	std::stringstream stream2;
	stream2 << 1000.0 * (std::clock() - started_at) / CLOCKS_PER_SEC << " ms";
	output["cputime"] = stream2.str();

	return true;
}

void Dumper::JobsHandler::format()
{
	const auto path = Dumper::Memory::get_dll_dir((HMODULE)Dumper::Memory::r_module);
	output["version"] = path.filename().string();
}