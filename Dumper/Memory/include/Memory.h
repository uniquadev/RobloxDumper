// This file is part of the uniquadev/RobloxDumper and is licensed under MIT License; see LICENSE.txt for details

#pragma once

#include <windows.h>
#include <filesystem>
#include <vector>

namespace Dumper::Memory
{
	inline DWORD r_procid;
	inline HANDLE r_process;
	
	inline uintptr_t r_module;
	inline uintptr_t r_module_end;

	inline size_t r_roblox_end;
	inline uintptr_t r_text_start;
	inline uintptr_t r_text_end;
	inline uintptr_t r_rdata_start;
	inline uintptr_t r_rdata_end;
	inline uintptr_t r_data_start;
	inline uintptr_t r_data_end;

	void init();
	std::filesystem::path get_dll_dir(HMODULE dllModule);

	/*
	* utils
	*/

	// retrive committed regions starting from addr to end_addr with the right protection flag
	std::vector<MEMORY_BASIC_INFORMATION>  get_regions(uintptr_t addr = 0, DWORD protect = PAGE_EXECUTE_READWRITE, uintptr_t end_addr = 0);
	// retrive addr region only if committed
	std::optional<MEMORY_BASIC_INFORMATION> get_region(uintptr_t addr);
}