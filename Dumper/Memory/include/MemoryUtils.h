// This file is part of the uniquadev/RobloxDumper and is licensed under MIT License; see LICENSE.txt for details

#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <stdint.h>

#include "Signature.h"

namespace Dumper::Memory
{
	class SearchSettings
	{
	public:
		uintptr_t start;
		uintptr_t end;
		DWORD protect;
		bool stop_first;
		bool fast_scan;
		SearchSettings(uintptr_t start, uintptr_t end, DWORD protect, bool stop_first, bool fast_scan)
			: start(start), end(end), protect(protect), stop_first(stop_first), fast_scan(fast_scan) {
		}
	};

	/// <summary>
	/// Subtract RobloxBase from passed addressed returing it's offset.
	/// </summary>
	uintptr_t get_offset(uintptr_t addr);

	/// <summary>
	/// Search all refs to the given address in the process memory.
	/// </summary>
	/// <returns></returns>
	std::vector<uintptr_t> get_xrefs(uintptr_t addr, SearchSettings settings);
	std::vector<uintptr_t> get_xrefs(uintptr_t addr);

	/// <summary>
	/// Search string in the read+write regions of the process, return its address.
	/// </summary>
	/// <returns></returns>
	std::vector<uintptr_t> find_string(const char* str, SearchSettings settings);
	std::vector<uintptr_t> find_string(const char* str);

	/// <summary>
	/// Return the address of the first 0xE8 byte
	/// </summary>
	/// <returns></returns>
	uintptr_t next_call(uintptr_t addr, size_t skips = 0);
	uintptr_t get_calling(uintptr_t call_add);

	/// <summary>
	/// Scan post addr memory until function prologue ops are matched
	/// </summary>
	/// <returns></returns>
	uintptr_t get_func_top(uintptr_t addr);

	/// <summary>
	/// Scan pre addr memory until function epilogue ops are matched
	///	</summary>
	uintptr_t get_func_end(uintptr_t addr);

	/// <summary>
	/// Scan the process memory to find the passed signature
	/// </summary>
	std::vector<uintptr_t> scan(std::string sign, SearchSettings settings);
	std::vector<uintptr_t> scan(Signature sign, SearchSettings settings);

	/// <summary>
	/// Search immediates values
	/// </summary>
	std::vector<int64_t> get_immediates(SearchSettings settings);
	std::vector<int64_t> get_immeoffsets(SearchSettings settings);
}