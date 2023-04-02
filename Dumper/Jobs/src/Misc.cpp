#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
using namespace Dumper;
using namespace Memory;

bool dump_misc(JobsHandler* h)
{
	h->push_addy("RobloxModule", r_module);

	// jmp esi
	auto jmp_sign = scan(
		"FF E6 C3",
		SearchSettings(r_module, r_module_end, PAGE_EXECUTE_READ, true)
	);
	if (jmp_sign.size() < 1)
		JOBERROR(h, "Unable to find JmpEsi signature");

	h->push_addy("JmpEsi", get_offset(jmp_sign[0]));
	return true;
}