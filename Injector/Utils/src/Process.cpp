#include "Process.h"
#include <TlHelp32.h>
#include <psapi.h> 

// constructors
Process::Process(uintptr_t id)
{
	this->id = (size_t)id;
	this->ph = OpenProcess(PROCESS_ALL_ACCESS, NULL, (DWORD)id);
	if (this->ph == INVALID_HANDLE_VALUE)
		throw std::exception("unable to open process");
	// retrive process base address
	std::filesystem::path path = GetPath();
	this->base = this->GetModuleBaseAddress(path.filename().wstring(), &this->base_size);
};

Process::~Process()
{
	CloseHandle(this->ph);
}

Process* Process::ByName(std::wstring process_name)
{
	// CreateToolhelp32Snapshot handle
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return NULL;
	// Create PROCESSENTRY32 structure
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	// Get first process
	if (!Process32First(hSnapshot, &pe32))
	{
		CloseHandle(hSnapshot);
		return NULL;
	}
	// Loop through all processes
	do
	{
		// convert pe32.szExeFile to wstring
		std::wstring name(pe32.szExeFile);
		// Compare process name
		if (process_name == name && pe32.th32ProcessID != 0)
		{
			// Close handle
			CloseHandle(hSnapshot);
			// Make new Process object
			return new Process(pe32.th32ProcessID);
		}
	} while (Process32Next(hSnapshot, &pe32));
	return NULL;
};

uintptr_t Process::GetModuleBaseAddress(std::wstring module_name, size_t* module_size)
{
	uintptr_t base = 0;
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, (DWORD)id);
	if (snap == INVALID_HANDLE_VALUE)
		return NULL;
	
	MODULEENTRY32 modEntry;
	modEntry.dwSize = sizeof(modEntry);
	if (Module32First(snap, &modEntry))
	{
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
		} while (Module32Next(snap, &modEntry));
	}
	CloseHandle(snap);
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

std::filesystem::path Process::GetPath()
{
	// retrive GetModuleFileNameEx from the ph
	char path[MAX_PATH];
	if (!GetModuleFileNameExA(ph, NULL, path, MAX_PATH))
		return "";
	return std::filesystem::path(path);
}
