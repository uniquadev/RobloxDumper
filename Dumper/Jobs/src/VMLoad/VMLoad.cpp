// This file is part of the uniquadev/RobloxDumper and is licensed under MIT License; see LICENSE.txt for details

#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
#include "JobsData.h"

using namespace Dumper;
using namespace Memory;

bool dump_vm_load(JobsHandler* h)
{
	auto old_res = find_string(
		"oldResult, moduleRef  = ...",
		SearchSettings(r_module, r_roblox_end, PAGE_READONLY, true, true)
	);
	if (old_res.size() < 1)
		JOBERROR(h, "Failed to find moduleRef string");

	auto refs = get_xrefs(
		old_res[0],
		SearchSettings(r_text_start, r_text_end, PAGE_EXECUTE_READ, true, false)
	);
	if (refs.size() < 1)
		JOBERROR(h, "Failed to find moduleRef string references");

	JobsData::modulescriptinternal_xref = refs[0];

	const auto sign = Signature("6A 00 83 C2 10 68 ? ? ? ? E8 ? ? ? ?");

	auto call_sign = scan(
		sign,
		SearchSettings(refs[0], r_roblox_end, PAGE_EXECUTE_READ, true, true)
	);
	if (call_sign.size() < 1)
		JOBERROR(h, "Could not find signature for LuaVM::load call");

	auto vmload = get_calling(next_call(call_sign[0]));
	h->push_sub("LuaVM::Load", get_offset(vmload));

	// store data
	JobsData::vmload_start = vmload;
	JobsData::vmload_end = get_func_end(vmload); // get_func_end must be improved for big/shuffled functions

	// luau_load inlined
	auto version_check = scan(
		"2C 03 0F 85 ? ? ? ?",
		SearchSettings(vmload, vmload + 3000, PAGE_EXECUTE_READ, true, false)
	);
	if (version_check.size() < 1)
		JOBERROR(h, "Could not find signature for lua_load inline");

	h->push_addy("inline_luau_load", get_offset(version_check[0]));

	return true;
}