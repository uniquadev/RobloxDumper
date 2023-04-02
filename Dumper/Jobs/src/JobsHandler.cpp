#include "JobsHandler.h"
#include "Jobs.h"

void Dumper::JobsHandler::register_jobs()
{
	jobs.push_back({ "Misc", dump_misc });
	jobs.push_back({ "Taskscheduler", dump_taskscheduler });
	jobs.push_back({ "LTypeName", dump_ltypename });
}