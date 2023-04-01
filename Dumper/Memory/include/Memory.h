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
	inline size_t r_size;

	void init();
	std::filesystem::path get_dll_dir(HMODULE dllModule);

	std::vector<MEMORY_BASIC_INFORMATION>  get_regions(uintptr_t addr = 0, DWORD protect = PAGE_EXECUTE_READWRITE);
	std::optional<MEMORY_BASIC_INFORMATION> get_region(uintptr_t addr);
}