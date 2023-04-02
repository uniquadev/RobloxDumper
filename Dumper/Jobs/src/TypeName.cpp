#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
using namespace Dumper;
using namespace Memory;

bool dump_ltypename(JobsHandler* h)
{
	auto typename_sign = scan(
		"B8 ? ? ? ? 83 ? FF 74 07 8B 04 95",
		SearchSettings(r_module, r_module_end, PAGE_EXECUTE_READ, true)
	);

	if (typename_sign.size() < 1)
		JOBERROR(h, "Failed to find lua_typename signature");

	uintptr_t lua_typename = get_offset(typename_sign[0]);
	h->push_addy("LuaTypeName", lua_typename);

	return true;
}