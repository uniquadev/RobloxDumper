#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
using namespace Dumper;
using namespace Memory;

bool dump_misc(JobsHandler* h)
{
	h->push_addy("RobloxModule", r_module);

	// retn
	// jmp     esi
	auto jmp_sign = scan(
		"C3 FF E6",
		SearchSettings(r_text_start, r_text_end, PAGE_EXECUTE_READ, true, false)
	);
	if (jmp_sign.size() < 1)
		JOBERROR(h, "Unable to find JmpEsi signature");

	h->push_addy("JmpEsi", get_offset(jmp_sign[0] + 1));
	return true;
}