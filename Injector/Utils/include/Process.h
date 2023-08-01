// This file is part of the uniquadev/RobloxDumper and is licensed under MIT License; see LICENSE.txt for details

#pragma once

#include <iostream>
#include <string>
#include <Windows.h>

/*
* custom process class 
*/
class Process
{
public:
	Process(uintptr_t id);
	~Process();
	// fields
	size_t id;
	uintptr_t base;
	size_t base_size;
	HANDLE ph;

	// methods
	uintptr_t GetModuleBaseAddress(std::wstring module_name, size_t* module_size = NULL);

	// ReadProcessMemory wrapper including safe memory reading
	bool ReadMemory(uintptr_t address, size_t size, void* buffer);

	std::wstring GetPath();
	static Process* ByName(std::wstring process_name);
};