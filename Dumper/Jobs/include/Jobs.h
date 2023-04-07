#pragma once

#include "JobsHandler.h"

#define DEFINE_JOB(name) bool dump_##name(Dumper::JobsHandler* h)
#define JOBERROR(h, x) { h->push_error(x); return false; }

DEFINE_JOB(misc);
DEFINE_JOB(taskscheduler);
DEFINE_JOB(ltypename);
DEFINE_JOB(scresume);
DEFINE_JOB(lua_openbase);
DEFINE_JOB(taskdefer);


DEFINE_JOB(vm_load);
DEFINE_JOB(pseudo2addr);
DEFINE_JOB(vmload_hooks);