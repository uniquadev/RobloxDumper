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
	// 85 ? ? 85 ? ? ? ? 8D ? ?
	// test    eax, eax
	// jnz     loc_20D09A4
	// lea     esi, [edi + ?] // ? = L_GLOBAL
	// deobf just after

	auto sign = scan(
		"85 ? ? 85 ? ? ? ? 8D ? ?",
		SearchSettings(os_date_start, os_date_end, PAGE_EXECUTE_READ, true, false)
	);

	if (sign.size() < 1)
		JOBERROR(h, "Can't find sign of L->global inside os_date");

	h->push_offset("L_GLOBAL", *(BYTE*)(sign[0] + 10));
	h->push_ptrobf("L_GLOBAL", PtrDeobf::get_ptrobf_type_str(sign[0] + 8));

	return true;
}