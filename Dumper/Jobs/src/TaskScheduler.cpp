// This file is part of the uniquadev/RobloxDumper and is licensed under MIT License; see LICENSE.txt for details

#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"

using namespace Dumper;
using namespace Memory;

bool dump_taskscheduler(JobsHandler* h)
{
	const auto settings = SearchSettings(r_text_start, r_text_end, PAGE_EXECUTE_READ, true, true);
	// find taskscheduler func
	auto func_sign = scan(
		"55 8B EC 83 E4 F8 83 EC 08 E8 ?? ?? ?? ?? 8D 0C 24",
		settings
	);
	if (func_sign.size() < 1)
		JOBERROR(h, "Failed to find taskscheduler caller signature");
	
	// first call is taskscheduler getter
	uintptr_t call = next_call(func_sign[0]);
	if (!call)
		JOBERROR(h, "Failed to find taskscheduler grabber call");
	
	// get taskscheduler
	uintptr_t taskscheduler = get_calling(call);
	if (taskscheduler == 0)
		JOBERROR(h, "Failed to find taskscheduler");
	h->push_sub("GetTaskScheduler", get_offset(taskscheduler));
	
	// find singleton offset
	const auto settings2 = SearchSettings(taskscheduler, taskscheduler + 300, PAGE_EXECUTE_READ, true, true);
	auto ret_sign = scan(
		"A1 ? ? ? ? 8B 4D F4 64 89 0D ? ? ? ? 8B E5 5D C3",
		settings2
	);
	if (ret_sign.size() < 1)
		JOBERROR(h, "Failed to find taskscheduler singleton signature");
	
	// get singleton offset
	uintptr_t singleton = get_offset(*(uintptr_t*)(ret_sign[0] + 1));
	h->push_addy("TaskSchedulerSingleton", singleton);
	return true;
}