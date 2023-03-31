#include "JobsHandler.h"
#include "Jobs.h"

void Dumper::JobsHandler::register_jobs()
{
	jobs.push_back({ "Taskscheduler", dump_taskscheduler });
}