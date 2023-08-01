// This file is part of the uniquadev/RobloxDumper and is licensed under MIT License; see LICENSE.txt for details

#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
#include "JobsData.h"
using namespace Dumper;
using namespace Memory;

using JobsData::startscript_start;
using JobsData::startscript_end;

bool dump_lgt(JobsHandler* h)
{
	auto sign = scan(
		"05 40 01 00 00 89 ? ?",
		SearchSettings(startscript_start, startscript_end, PAGE_EXECUTE_READ, true, false)
	);

	if (sign.size() < 1)
		JOBERROR(h, "Failed to find Lstate base usage signature");

	// Next 89 mov op is L_GLOBALTABLE usage, L1->gt = L->gt;
	BYTE L_GLOBALTABLE = 0;
	for (size_t i = sign[0] + 9; i < sign[0] + 20; i++)
	{
		if (*(BYTE*)i == 0x89)
		{
			L_GLOBALTABLE = *(BYTE*)(i + 2);
			break;
		}
	}
	if (L_GLOBALTABLE == 0)
		JOBERROR(h, "Failed to find L_GLOBALTABLE offset");

	h->push_offset("L_GT", L_GLOBALTABLE);

	return true;
}