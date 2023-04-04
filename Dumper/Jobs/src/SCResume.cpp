#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
using namespace Dumper;
using namespace Memory;

bool dump_scresume(JobsHandler* h)
{
	auto str = find_string(
		"Maximum re-entrancy depth (%i) exceeded",
		SearchSettings(r_rdata_start, r_rdata_end, PAGE_READONLY, true, true)
	);

	if (str.size() < 1)
		JOBERROR(h, "Unable to find re-entrancy string");


	auto xrefs = get_xrefs(
		str[0],
		SearchSettings(r_text_start, r_text_end, PAGE_EXECUTE_READ, false, false)
	);

	if (xrefs.size() < 1)
		JOBERROR(h, "Unable to find xref to re-entrancy");

	// sc_resume is the one with xref near func top
	uintptr_t sc_resume = 0;
	size_t best = MAXUINT;
	for (const auto& ref : xrefs) // we loop re-entrancy string xref to find the right xref
	{
		auto top = get_func_top(ref);
		auto size = ref - top;
		if (size < best)
		{
			sc_resume = top;
			best = size;
		}
	}

	h->push_sub(
		"ScriptContextResume", 
		get_offset(sc_resume)
	);

	return true;
}