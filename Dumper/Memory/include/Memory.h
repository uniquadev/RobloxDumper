#pragma once

#include <windows.h>
#include <filesystem>

namespace Dumper::Memory
{
	inline DWORD r_procid;
	inline HANDLE r_process;
	inline HMODULE r_module;
	inline size_t r_size;

	void init();
	std::filesystem::path get_dll_dir(HMODULE dllModule);
}