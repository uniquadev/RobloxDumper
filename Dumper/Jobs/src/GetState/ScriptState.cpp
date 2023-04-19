#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
#include "JobsData.h"
#include "PtrDeobf.h"

using namespace Dumper;
using namespace Memory;

using JobsData::getstate;

bool dump_scriptstate(JobsHandler* h) {
	// because of multiple return statements producing multiple epilogues, get_func_end wont work here
	auto scriptstate_calls = scan(
		"E8 ? ? ? ? 8B ? ? 5F 5E",
		SearchSettings(getstate, getstate + 0x1000, PAGE_EXECUTE_READ, true, true)
	);
	if (scriptstate_calls.size() < 1)
		JOBERRORF(h, "Could not find signature for getscriptstate call (Scanned from {:X} to {:X})", getstate, getstate + 0x1000);

	auto getstate = get_calling(scriptstate_calls[0]);

	uintptr_t get_scriptstate = get_calling(scriptstate_calls[0]);

	h->push_sub("GetScriptState", get_offset(get_scriptstate));
	
	auto case_jmp = scan(
		"FF ? ? ? ? ? ?"" ""B8",
		SearchSettings(get_scriptstate, get_func_end(get_scriptstate), PAGE_EXECUTE_READ, true, true)
	);
	if (case_jmp.size() < 1)
		JOBERROR(h, "Could not find case jmp for getscriptstate call");

	uintptr_t* jmp_table = *reinterpret_cast<uintptr_t**>(case_jmp[0] + 3);
	uintptr_t script_case = jmp_table[2];

	h->push_offset("SC_SCRIPTSTATE", *reinterpret_cast<uintptr_t*>(script_case + 1));
	h->push_ptrobf("SC_SCRIPTSTATE", PtrDeobf::get_ptrobf_type_str(script_case));

	return true;
}