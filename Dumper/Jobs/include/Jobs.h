#pragma once

#include "JobsHandler.h"

#define DEFINE_JOB(name) bool dump_##name(Dumper::JobsHandler* h)
#define JOBERROR(h, x) { h->push_error(x); return false; }
#define JOBERRORF(h, x, ...) { h->push_error(std::format(x, __VA_ARGS__)); return false; }

DEFINE_JOB(misc);
DEFINE_JOB(taskscheduler);
DEFINE_JOB(ltypename);
DEFINE_JOB(scresume);
DEFINE_JOB(lua_openbase);
DEFINE_JOB(taskdefer);
DEFINE_JOB(getstate);
DEFINE_JOB(printf);

DEFINE_JOB(vm_load);
DEFINE_JOB(pseudo2addr);
DEFINE_JOB(vmload_hooks);

DEFINE_JOB(os_date);
DEFINE_JOB(lglobal);

DEFINE_JOB(getstate);
DEFINE_JOB(scriptstate);