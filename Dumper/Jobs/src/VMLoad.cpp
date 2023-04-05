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
	JobsData::vmload_end = get_func_end(vmload) - vmload;

	h->push_offset("LuaVM::Load_Size", JobsData::vmload_end);

	// luau_load inlined
	auto version_check = scan(
		"2C 03 0F 85 ? ? ? ?",
		SearchSettings(vmload, vmload + 3000, PAGE_EXECUTE_READ, true, false)
	);
	if (version_check.size() < 1)
		JOBERROR(h, "Could not find signature for lua_load inline");

	h->push_addy("luau_load", get_offset(version_check[0]));

	// luavm_load_hashcheck_hook
	auto exit_routine = scan(
		"8D ? ? ? ? ? E8 ? ? ? ? 8D ? ? ? ? ? E8 ? ? ? ?",
		SearchSettings(vmload, vmload + 0x6000, PAGE_EXECUTE_READ, true, false)
	);
	if (exit_routine.size() < 1)
		JOBERROR(h, "Could not find signature for luavm_load_hashcheck_hook");

	h->push_addy("luavm_load_hashcheck_hook", get_offset(exit_routine[0]));

	// luavm_load_bytecode_hook
	auto decompression_end = scan(
		"8B ? ? ? ? ? 8B ? ? ? ? ? 2B ?",
		SearchSettings(vmload, vmload + 0x6000, PAGE_EXECUTE_READ, true, false)
	);
	if (decompression_end.size() < 1)
		JOBERROR(h, "Could not find signature for luavm_load_bytecode_hook");

	h->push_addy("luavm_load_bytecode_hook", get_offset(decompression_end[0]));

	// luavm_load stackframe
	unsigned int bytecode_offset{}, bytecode_len_offset{}; // these will be set to disps of ebp
	auto arg_pusher = scan(
		"8B ? ? 8B ? ? ? ? ? ? ? E8 ? ? ? ? 83 C4 08",
		SearchSettings(vmload, vmload + 0x6000, PAGE_EXECUTE_READ, true, false)
	);
	if (arg_pusher.size() < 1) {
		JOBERROR(h, "Could not find signature for luavm_load_bytecode_hook");
	} else {
		// edx, ecx
		bytecode_len_offset = ((~*reinterpret_cast<unsigned int*>(arg_pusher[0] + 2)) & 0xFF) + 1; // single byte displacement
		bytecode_offset = ~*reinterpret_cast<unsigned int*>(arg_pusher[0] + 5) + 1;
	}

	h->push_offset("luavm_load_stackframe_bytecode", bytecode_offset);
	h->push_offset("luavm_load_stackframe_bytecode_len", bytecode_len_offset);

	return true;
}