#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
using namespace Dumper;
using namespace Memory;

// top - base
// 8B ? 18 8B ? 8B ? 14 2B ?
//		^			 ^

bool dump_lbase(JobsHandler* h)
{
	auto str = find_string(
		"Color3.toHSV requires a Color3 argument",
		SearchSettings(r_rdata_start, r_rdata_end, PAGE_READONLY, true, false)
	);
	if (str.size() < 1)
		JOBERROR(h, "Couldn't find Color3.toHSV string.");

	auto xref = get_xrefs(
		str[0], SearchSettings(r_text_start, r_text_end, PAGE_EXECUTE_READ, true, false)
	);
	if (xref.size() < 1)
		JOBERROR(h, "Couldn't find Color3.toHSV xref.");

	auto tohsv_top = get_func_top(xref[0]);
	auto sign = scan(
		"8B ? ? 8B ? 8B ? ? 2B ?",
		SearchSettings(tohsv_top, tohsv_top + 100, PAGE_EXECUTE_READ, true, false)
	);
	if (sign.size() < 1)
		JOBERROR(h, "Couldn't find top-base sign.");

	h->push_offset("L_BASE", *(BYTE*)(sign[0] + 7));

	return true;
}