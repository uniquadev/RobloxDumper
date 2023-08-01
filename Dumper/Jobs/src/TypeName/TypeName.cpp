// This file is part of the uniquadev/RobloxDumper and is licensed under MIT License; see LICENSE.txt for details

#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
#include "JobsData.h"
using namespace Dumper;
using namespace Memory;


/*
*	Another way could be searching for "no value" string and xref it to get inside typename
*/

bool dump_ltypename(JobsHandler* h)
{
	auto typename_sign = scan(
		"B8 ? ? ? ? 83 ? FF 74 07 8B 04 95",
		SearchSettings(r_text_start, r_text_end, PAGE_EXECUTE_READ, true, true)
	);

	if (typename_sign.size() < 1)
		JOBERROR(h, "Failed to find lua_typename signature");

	uintptr_t lua_typename = get_offset(typename_sign[0]);
	h->push_sub("LuaTypeName", lua_typename);

	JobsData::typename_start = typename_sign[0];

	return true;
}