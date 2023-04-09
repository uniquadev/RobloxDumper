#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
using namespace Dumper;
using namespace Memory;

bool dump_printf(JobsHandler* h)
{
	auto requestbudget = find_string(
		"GetRequestBudgetForRequestType: Unknown request type.",
		SearchSettings(r_rdata_start, r_rdata_end, PAGE_READONLY, true, false)
	);
	if (requestbudget.size() < 1)
		JOBERROR(h, "Could not find GetRequestBudgetForRequestType");

	auto refs = get_xrefs(
		requestbudget[0],
		SearchSettings(r_text_start, r_text_end, PAGE_EXECUTE_READ, true, false)
	);
	if (refs.size() < 1)
		JOBERROR(h, "Could not find refs for requestbudget");

	uintptr_t printf_addr = get_calling(next_call(refs[0]));
	h->push_sub("Printf", get_offset(printf_addr));

	return true;
}