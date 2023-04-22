#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
#include "JobsData.h"
using namespace Dumper;
using namespace Memory;

using JobsData::os_date_start;
using JobsData::os_date_end;

bool dump_dummynode(JobsHandler* h)
{
	auto sign = scan(
		"83 7F 0C 00 75 0C 81 FF ? ? ? ?",
		SearchSettings(
			os_date_start, os_date_end, PAGE_EXECUTE_READ, true, false
		)
	);
	if (sign.size() < 1)
		JOBERROR(h, "Can't find dummynode compare");

	h->push_addy("dummynode", get_offset(*(uintptr_t*)(sign[0] + 8)));
	return true;
}