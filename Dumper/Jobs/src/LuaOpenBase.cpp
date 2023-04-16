#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
#include "JobsData.h"
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

	JobsData::luaopen_base_start = get_func_top(version_str[0]);

	h->push_sub(
		"luaopen_base",
		get_offset(
			JobsData::luaopen_base_start
		)
	);

	JobsData::luaopen_base_end = get_func_end(version_str[0]);

	return true;
}