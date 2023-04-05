#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
using namespace Dumper;
using namespace Memory;

bool dump_lua_openbase(JobsHandler* h)
{
	auto version_str = scan(
		"5F 56 45 52 C7",
		SearchSettings(r_module, r_module_end, PAGE_EXECUTE_READ, true, false)
	);

	if (version_str.size() < 1)
		JOBERROR(h, "Can't find _VERSION string");

	h->push_sub(
		"lua_openbase",
		get_offset(
			get_func_top(version_str[0])
		)
	);

	return true;
}