#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
#include "JobsData.h"
using namespace Dumper;
using namespace Memory;

using JobsData::vmload_start;

bool dump_vmload_hooks(JobsHandler* h)
{
	// luavm_load_hashcheck_hook
	auto exit_routine = scan(
		"8D ? ? ? ? ? E8 ? ? ? ? 8D ? ? ? ? ? E8 ? ? ? ?",
		SearchSettings(vmload_start, vmload_start + 0x6000, PAGE_EXECUTE_READ, true, false)
	);
	if (exit_routine.size() < 1)
		JOBERROR(h, "Could not find signature for luavm_load_hashcheck_hook");

	h->push_addy("luavm_load_hashcheck_hook", get_offset(exit_routine[0]));

	// luavm_load_bytecode_hook
	auto decompression_end = scan(
		"8B ? ? ? ? ? 8B ? ? ? ? ? 2B ?",
		SearchSettings(vmload_start, vmload_start + 0x6000, PAGE_EXECUTE_READ, true, false)
	);
	if (decompression_end.size() < 1)
		JOBERROR(h, "Could not find signature for luavm_load_bytecode_hook");

	h->push_addy("luavm_load_bytecode_hook", get_offset(decompression_end[0]));

	// luavm_load stackframe
	int bytecode_offset{}, bytecode_len_offset{}; // these will be set to disps of ebp
	auto arg_pusher = scan(
		"8B ? ? 8B ? ? ? ? ? ? ? E8 ? ? ? ? 83 C4 08",
		SearchSettings(vmload_start, vmload_start + 0x6000, PAGE_EXECUTE_READ, true, false)
	);
	if (arg_pusher.size() < 1) {
		JOBERROR(h, "Could not find signature for luavm_load_bytecode_hook");
	}
	else {
		// edx, ecx
		bytecode_len_offset = static_cast<signed int>(*reinterpret_cast<signed char*>(arg_pusher[0] + 2)); // single byte displacement
		bytecode_offset = *reinterpret_cast<signed int*>(arg_pusher[0] + 5);
	}

	h->push_offset("luavm_load_stackframe_bytecode", bytecode_offset);
	h->push_offset("luavm_load_stackframe_bytecode_len", bytecode_len_offset);

	return true;
}