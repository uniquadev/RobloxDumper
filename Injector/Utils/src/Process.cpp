// This file is part of the uniquadev/RobloxDumper and is licensed under MIT License; see LICENSE.txt for details

#include "Process.h"

#include <TlHelp32.h>
#include <psapi.h> 
#include <filesystem>

Process::~Process()
{
	CloseHandle(this->ph);
}

Process::Process(uintptr_t id)
{
	this->id = static_cast<size_t>(id);

	// open process with all access
	this->ph = OpenProcess(PROCESS_ALL_ACCESS, NULL, (DWORD)id);
	if (this->ph == INVALID_HANDLE_VALUE)
		throw std::exception("unable to open process");

	// retrive process base address
	const auto path = GetPath();
	this->base = this->GetModuleBaseAddress(path, &this->base_size); // retrive exe base address
};

Process* Process::ByName(std::wstring process_name)
{
	// CreateToolhelp32Snapshot handle
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return NULL;

	// create PROCESSENTRY32 structure
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// get first process
	if (!Process32First(hSnapshot, &pe32))
	{
		CloseHandle(hSnapshot);
		return NULL;
	}

	// loop through all processes
	do
	{
		// convert pe32.szExeFile to wstring
		std::wstring name(pe32.szExeFile);
		// compare process name
		if (process_name == name && pe32.th32ProcessID != 0)
		{
			// close handle
			CloseHandle(hSnapshot);
			// make new Process object
			return new Process(pe32.th32ProcessID);
		}
	} while (Process32Next(hSnapshot, &pe32));

	CloseHandle(hSnapshot);
	return NULL;
};

uintptr_t Process::GetModuleBaseAddress(std::wstring module_name, size_t* module_size)
{
	uintptr_t base = 0;

	// CreateToolhelp32Snapshot handle
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, static_cast<DWORD>(id));
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return NULL;
	
	// get first module
	MODULEENTRY32 modEntry = {};
	modEntry.dwSize = sizeof(modEntry);
	if (!Module32First(hSnapshot, &modEntry))
	{
		CloseHandle(hSnapshot);
		return 0;
	}

	// loop through all modules
	do
	{
		std::wstring name(modEntry.szModule);
		if (module_name == name)
		{
			if (module_size)
				*module_size = (size_t)modEntry.modBaseSize;
			base = (uintptr_t)modEntry.modBaseAddr;
			break;
		}
	} while (Module32Next(hSnapshot, &modEntry));
	
	CloseHandle(hSnapshot);
	return base;
}

bool Process::ReadMemory(uintptr_t address, size_t size, void* buffer)
{
	// virtual protect memory
	DWORD old_protect;
	if (!VirtualProtectEx(this->ph, (LPVOID)address, size, PAGE_EXECUTE_READWRITE, &old_protect))
		return false;

	// read memory
	bool flag = ReadProcessMemory(this->ph, (void*)address, buffer, size, NULL);

	// restore memory protection
	VirtualProtectEx(this->ph, (LPVOID)address, size, old_protect, &old_protect);
	return true;
}

std::wstring Process::GetPath()
{
	// retrive GetModuleFileNameEx from the ph
	char path[MAX_PATH];
	if (!GetModuleFileNameExA(ph, NULL, path, MAX_PATH))
		return L"";
	return std::filesystem::path(path).wstring();
}
