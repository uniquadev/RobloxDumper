// This file is part of the uniquadev/RobloxDumper and is licensed under MIT License; see LICENSE.txt for details

#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
#include "JobsData.h"
#include "PtrDeobf.h"
using namespace Dumper;
using namespace Memory;

using JobsData::luaopen_base_start;
using JobsData::luaopen_base_end;

// B8 5F 56 45 52 3B ? 14
// *(_DWORD *)(tversion + 20) != 'REV_'

bool dump_tssize(JobsHandler* h)
{
	h->push_offset("TS_DATA", 0x14); // static offset

	auto sign = scan(
		"B8 5F 56 45 52 3B ? 14",
		SearchSettings(luaopen_base_start, luaopen_base_end, PAGE_EXECUTE_READ, true, false)
	);

	if (sign.size() < 1)
		JOBERROR(h, "Can't find signature to ts->data != ...");

	JobsData::ts_version_check_start = sign[0];

	auto movs = scan(
		"8B",
		SearchSettings(sign[0] - 15, sign[0], PAGE_EXECUTE_READ, true, false)
	);

	if (movs.size() != 1)
		JOBERROR(h, "Can't find any mov opcode before ts->data compare");

	auto offsets = get_immeoffsets(
		SearchSettings(movs[0], sign[0], PAGE_EXECUTE_READ, true, false)
	);

	if (offsets.size() < 1)
		JOBERROR(h, "Can't find tsize offsets");

	h->push_offset("TS_SIZE", offsets[0]);
	h->push_ptrobf("TS_SIZE", PtrDeobf::get_ptrobf_type_str(movs[0]));

	return true;
}