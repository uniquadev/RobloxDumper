// This file is part of the uniquadev/RobloxDumper and is licensed under MIT License; see LICENSE.txt for details

#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
#include "JobsData.h"
#include "PtrDeobf.h"

using namespace Dumper;
using namespace Memory;

using JobsData::os_date_start;
using JobsData::os_date_end;

bool dump_lglobal(JobsHandler* h)
{
	// 85 ? 0F 85 ? ? ? ?
	// 85 ? ? 85 ? ? ? ? 8D ? ?
	// test    eax, eax
	// jnz     loc_20D09A4
	// lea     esi, [edi + ?] // ? = L_GLOBAL
	// deobf just after

	auto sign = scan(
		"85 ? 0F 85 ? ? ? ?",
		SearchSettings(os_date_start, os_date_end, PAGE_EXECUTE_READ, true, false)
	);

	if (sign.size() < 1)
		JOBERROR(h, "Can't find sign of L->global inside os_date");

	auto imm = get_immeoffsets(
		SearchSettings(sign[0] + 8, sign[0] + 30, PAGE_EXECUTE_READ, true, false)
	);

	if (imm.size() < 1)
		JOBERROR(h, "Can't find immediates after L->global sign in os_date");

	h->push_offset("L_GLOBAL", imm[0]);
	h->push_ptrobf("L_GLOBAL", PtrDeobf::get_ptrobf_type_str(sign[0] + 8));

	return true;
}