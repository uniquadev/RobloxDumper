#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
using namespace Dumper;
using namespace Memory;


/*
*	Another way could be searching for "no value" string and xref it to get inside typename
*/

bool dump_taskdefer(JobsHandler* h) {
	auto old_res = find_string(
		"Maximum re-entrancy depth (%i) exceeded calling task.defer",
		SearchSettings(r_module, r_roblox_end, PAGE_READONLY, true, true)
	);
	if (old_res.size() < 1)
		JOBERROR(h, "Failed to find re-entrancy string");

	auto refs = get_xrefs(
		old_res[0],
		SearchSettings(r_text_start, r_text_end, PAGE_EXECUTE_READ, true, false)
	);
	if (refs.size() < 1)
		JOBERROR(h, "Failed to find re-entrancy string references");

	uintptr_t defer = Dumper::Memory::get_func_top(refs[0]);

	if (!defer)
		JOBERROR(h, "Failed to find prologue");

	h->push_sub("TaskDefer", get_offset(defer));

	return true;
}