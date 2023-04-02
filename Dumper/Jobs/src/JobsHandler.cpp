#include "JobsHandler.h"
#include "Jobs.h"

void Dumper::JobsHandler::register_jobs()
{
	jobs.push_back({ "Misc", dump_misc });
	jobs.push_back({ "Taskscheduler", dump_taskscheduler });
	jobs.push_back({ "LTypeName", dump_ltypename });
	jobs.push_back({ "LuaVM::Load", dump_vm_load });
}