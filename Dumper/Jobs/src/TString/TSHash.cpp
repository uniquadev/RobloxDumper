// This file is part of the uniquadev/RobloxDumper and is licensed under MIT License; see LICENSE.txt for details

#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
#include "JobsData.h"
#include "PtrDeobf.h"
using namespace Dumper;
using namespace Memory;

using JobsData::ts_version_check_start;

bool dump_tshash(JobsHandler* h)
{
	auto s = Signature("B8 ? 80 FF FF 66 ? ? ?");
	auto atom = scan(
		s,
		SearchSettings(ts_version_check_start, ts_version_check_start + 300, PAGE_EXECUTE_READ, true, false)
	);

	if (atom.size() < 1)
		JOBERROR(h, "Can't find tsatom signature inside luaopen_base");

	h->push_offset("TS_ATOM", *(BYTE*)(atom[0] + s.sign.size() - 1));

	auto offsets = get_immeoffsets(
		SearchSettings(atom[0] + s.sign.size(), atom[0] + s.sign.size() + 10, PAGE_EXECUTE_READ, true, false)
	);

	if (offsets.size() < 1)
		JOBERROR(h, "Insufficent size of offsets after atom signature");

	h->push_offset("TS_HASH", offsets[0]);
	h->push_ptrobf("TS_HASH", PtrDeobf::get_ptrobf_type_str(atom[0] + s.sign.size(), 40));

	return true;
}