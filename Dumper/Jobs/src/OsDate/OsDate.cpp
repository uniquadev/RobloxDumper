// This file is part of the uniquadev/RobloxDumper and is licensed under MIT License; see LICENSE.txt for details

#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
#include "JobsData.h"
using namespace Dumper;
using namespace Memory;

bool dump_os_date(JobsHandler* h)
{
	auto wday = find_string(
		"wday",
		SearchSettings(r_rdata_start, r_rdata_end, PAGE_READONLY, true, false)
	);

	if (wday.size() < 1)
		JOBERROR(h, "Can't find wday string in rdata memory");

	auto xref = get_xrefs(
		wday[0],
		SearchSettings(r_text_start, r_text_end, PAGE_EXECUTE_READ, true, false)
	);

	if (xref.size() < 1)
		JOBERROR(h, "Can't find wday xref in text memory");

	JobsData::os_date_start = get_func_top(xref[0]);
	JobsData::os_date_end = get_func_end(xref[0]);

	h->push_sub("os_date", get_offset(JobsData::os_date_start));

	return true;
}