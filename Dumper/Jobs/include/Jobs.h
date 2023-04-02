#pragma once

#include "JobsHandler.h"

#define DEFINE_JOB(name) bool dump_##name(Dumper::JobsHandler* h)
#define JOBERROR(h, x) { h->output["Errors"].push_back(x); return false; }

DEFINE_JOB(misc);
DEFINE_JOB(taskscheduler);
DEFINE_JOB(ltypename);