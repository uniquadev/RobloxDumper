#include <string>
#include <Windows.h>
#include <filesystem>

#pragma once
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
	bool ReadMemory(uintptr_t address, size_t size, void* buffer);
	std::filesystem::path GetPath();
	static Process* ByName(std::wstring process_name);
};