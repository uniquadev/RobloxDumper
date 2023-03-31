#pragma once

#include "JobsHandler.h"

#define DEFINE_JOB(name) bool dump_##name(Dumper::JobsHandler* h)

DEFINE_JOB(taskscheduler);