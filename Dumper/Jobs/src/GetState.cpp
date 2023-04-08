#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
#include "JobsData.h"

using namespace Dumper;
using namespace Memory;

bool dump_getstate(JobsHandler* h) {
	auto old_res = find_string(
		"startScript re-entrancy has exceeded 3",
		SearchSettings(r_module, r_roblox_end, PAGE_READONLY, true, true)
	);
	if (old_res.size() < 1)
		JOBERROR(h, "Failed to find startScript re-entrancy string");

	auto refs = get_xrefs(
		old_res[0],
		SearchSettings(r_text_start, r_text_end, PAGE_EXECUTE_READ, true, false)
	);
	if (refs.size() < 1)
		JOBERROR(h, "Failed to find startScript re-entrancy string references");

	JobsData::startscript_start = get_func_top(refs[0]);
	JobsData::startscript_end = get_func_end(refs[0]);
	
	auto getstate_calls = scan(
		"E8 ? ? ? ?"" ""8B ?"" ""8D 8D ? ? ? ?"" ""68 ? ? ? ?",
		SearchSettings(JobsData::startscript_start, JobsData::startscript_end, PAGE_EXECUTE_READ, true, true)
	);
	if (getstate_calls.size() < 1)
		JOBERROR(h, "Could not find signature for getstate call");
	if (getstate_calls.size() > 1)
		JOBERROR(h, "Found too many signatures for getstate call");

	auto getstate = get_calling(getstate_calls[0]);

	h->push_sub("GetState", get_offset(getstate));

	return true;
}