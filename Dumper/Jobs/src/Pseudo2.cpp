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
		SearchSettings(modulescriptinternal_xref, get_func_end(modulescriptinternal_xref), PAGE_EXECUTE_READ, true, true)
	);

	if (sign.size() < 1)
		JOBERROR(h, "Unable to find pseudo2addr call inside modulescriptinternal");

	JobsData::pseudo2addr = get_calling(next_call(sign[0]));

	h->push_sub("pseudo2addr", get_offset(JobsData::pseudo2addr));

	return true;
}