#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
#include "JobsData.h"
#include "PtrDeobf.h"

using namespace Dumper;
using namespace Memory;

using JobsData::os_date_start;
using JobsData::os_date_end;

bool dump_ltop(JobsHandler* h)
{
	// simple look for top incrementation
	// add	dword ptr [edi+18h], 10h
	auto sign = scan(
		"83 ? ? 10",
		SearchSettings(os_date_start, os_date_end, PAGE_EXECUTE_READ, true, false)
	);

	if (sign.size() < 1)
		JOBERROR(h, "Can't find sign of L->top++ inside os_date");

	h->push_offset("L_TOP", *(BYTE*)(sign[0] + 2));

	return true;
}