#pragma once
#include "Signature.h"
#include "Process.h"

struct MemoryRegion
{
	uintptr_t base;
	size_t size;
	MemoryRegion(uintptr_t base, size_t size) {
		this->base = base;
		this->size = size;
	}
};


class ExternalDumper
{
public:
	Process* p;
	ExternalDumper(Process* p) { this->p = p; };
	__inline uintptr_t GetOffset(uintptr_t addr) { return addr - p->base; };
	/// <summary>
	/// Retrive a vector containing all memory regions of the process with the given protection
	/// </summary>
	/// <returns></returns>
	std::vector<MemoryRegion> GetMemoryRegions(DWORD protect, uintptr_t base=0);
	MemoryRegion GetMemoryRegion(uintptr_t addr);
	/// <summary>
	/// Search all refs to the given address in the process memory.
	/// </summary>
	/// <returns></returns>
	std::vector<uintptr_t> GetRefs(uintptr_t addr, bool stop_first = true);
	/// <summary>
	/// Search string in the read+write regions of the process, return its address.
	/// </summary>
	/// <returns></returns>
	std::vector<uintptr_t> FindString(const char* str, bool stop_first=true);
	/// <summary>
	/// Return the address of the first 0xE8 byte
	/// </summary>
	/// <returns></returns>
	uintptr_t NextCall(uintptr_t addr, size_t skips=0);
	uintptr_t GetCalling(uintptr_t addr);
	/// <summary>
	/// Scan post addr memory until function prologue ops are matched
	/// </summary>
	/// <returns></returns>
	uintptr_t GetFuncTop(uintptr_t addr);
	/// <summary>
	/// Scan the process memory to find the passed signature
	/// </summary>
	/// <returns></returns>
	std::vector<uintptr_t> SignatureScan(std::string ida_sign, uintptr_t from=0, uintptr_t end=0, bool stop_first=true);
	std::vector<uintptr_t> SignatureScan(Signature sign, uintptr_t from=0, uintptr_t end=0, bool stop_first=true);
};

