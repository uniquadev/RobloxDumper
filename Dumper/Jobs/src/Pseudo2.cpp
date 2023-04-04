#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
#include "JobsData.h"
using namespace Dumper;
using namespace Memory;

using JobsData::modulescriptinternal_xref;

bool dump_pseudo2addr(JobsHandler* h)
{
	auto sign = scan(
		"BA ? ? FF FF 8B CF E8 ? ? ? ?",
		SearchSettings(modulescriptinternal_xref, modulescriptinternal_xref + 300, PAGE_EXECUTE_READ, true, true)
	);

	if (sign.size() < 1)
		JOBERROR(h, "Unable to find pseudo2addr call inside modulescriptinternal");

	JobsData::pseudo2addr_start = get_calling(next_call(sign[0]));
	JobsData::pseudo2addr_end = get_func_end(JobsData::pseudo2addr_start);

	h->push_sub("pseudo2addr", get_offset(JobsData::pseudo2addr_start));

	auto nilObjectSign = scan(
		"B8 ? ? ? ? 5E C3",
		SearchSettings(JobsData::pseudo2addr_start, JobsData::pseudo2addr_end, PAGE_EXECUTE_READ, true, true)
	);

	if (nilObjectSign.size() < 1)
		JOBERROR(h, "Failed to find nilObjectSign signature");

	intptr_t offset = *(intptr_t*)(nilObjectSign[0] + 0x1);
	h->push_offset("nilObject", get_offset(offset));

	return true;
}