#include "Dumper.h"




std::vector<MemoryRegion> ExternalDumper::GetMemoryRegions(DWORD protect, uintptr_t base)
{
	auto res = std::vector<MemoryRegion>();
	MEMORY_BASIC_INFORMATION mbi;
	base = base == 0 ? this->p->base : base;
	while (VirtualQueryEx(this->p->ph, (LPCVOID)base, &mbi, sizeof(mbi)) != 0)
	{
		if (mbi.State == MEM_COMMIT && (mbi.Protect & protect))
		{
			res.push_back(MemoryRegion(base, mbi.RegionSize));
		}
		base += (DWORD)mbi.RegionSize;
	}

	return res;
}

MemoryRegion ExternalDumper::GetMemoryRegion(uintptr_t addr)
{
	MemoryRegion res = MemoryRegion(0, 0);
	MEMORY_BASIC_INFORMATION mbi;
	if (VirtualQueryEx(this->p->ph, (LPCVOID)addr, &mbi, sizeof(mbi)) != 0)
	{
		if (mbi.State == MEM_COMMIT)
		{
			res.base = (uintptr_t)mbi.BaseAddress;
			res.size = mbi.RegionSize;
		}
	}

	return res;
}

std::vector<uintptr_t> ExternalDumper::GetRefs(uintptr_t addr, bool stop_first)
{
	auto res = std::vector<uintptr_t>();
	auto regions = this->GetMemoryRegions(PAGE_EXECUTE_READ);

	// foreach execute regions and find refs address
	for (auto& region : regions)
	{
		auto buf = std::vector<char>(region.size);
		if (!p->ReadMemory(region.base, region.size, buf.data()))
			continue;

		for (size_t i = 0; i < region.size; i++) // since asm op no align scan
		{
			uintptr_t addr2 = *(uintptr_t*)(buf.data() + i);
			if (addr == addr2)
			{
				res.push_back(region.base + i);
				if (stop_first)
					return res;
			}
		}
	}
	return res;
}

std::vector<uintptr_t> ExternalDumper::FindString(const char* str, bool stop_first)
{
	auto res = std::vector<uintptr_t>();
	auto regions = this->GetMemoryRegions(PAGE_READONLY);
	
	// foreach regions and ReadProcessMemory for each region and search for str
	for (auto& region : regions)
	{
		auto buf = std::vector<char>(region.size);
		if (!p->ReadMemory(region.base, region.size, buf.data()))
			continue;
		
		for (size_t i = 0; i < region.size; i+=sizeof(char*))
		{
			if (memcmp(buf.data() + i, str, strlen(str)) == 0)
			{
				res.push_back(region.base + i);
				if (stop_first)
					return res;
			}
		}
	}
	
	return res;
}

uintptr_t ExternalDumper::NextCall(uintptr_t addr, size_t skips)
{
	BYTE op;
	MEMORY_BASIC_INFORMATION mbi;
	size_t current = 0;
	// get current region
	while (VirtualQueryEx(this->p->ph, (LPCVOID)addr, &mbi, sizeof(mbi)) != 0)
	{
		// read region in a buf
		auto buf = std::vector<BYTE>(mbi.RegionSize);
		uintptr_t base = (uintptr_t)mbi.BaseAddress;
		if (!p->ReadMemory(base, mbi.RegionSize, buf.data())) // read region
			continue;
		// loop buf and check if op match call
		for (size_t i = addr-base; i < mbi.RegionSize; i++)
		{
			op = buf[i];
			if (op == 0xE8)
			{
				if (current == skips)
					return base + i;
				else
					current++;
			}
		}
		
		addr += (uintptr_t)mbi.RegionSize;
	}
	return 0;
}

uintptr_t ExternalDumper::GetCalling(uintptr_t addr)
{
	intptr_t off = 0;
	// read addr+1 from process memory
	if (!p->ReadMemory(addr + 1, sizeof(off), &off))
		return 0;
	
	return addr + off + 5 ;
}

uintptr_t ExternalDumper::GetFuncTop(uintptr_t addr)
{
	const BYTE prol[3] = { 0x55, 0x8B, 0xEC }; // prologue
	uintptr_t base = addr;
	while (true)
	{
		auto region = GetMemoryRegion(base);
		if (region.size == 0) // check if region found
			break;
		// read region in buf
		auto buf = std::vector<BYTE>(region.size);
		if (!p->ReadMemory(region.base, region.size, buf.data()))
			break;
		// for back the addr
		for (size_t i = addr - region.base; i > 0; i--)
		{
			size_t addr2 = region.base + i;
			// compare
			if (memcmp(buf.data() + i, prol, sizeof(prol)) == 0)
				return addr2; // found
		}
		base = region.base - 1; // back to new region
	}
	// default ret
	return addr;
}

std::vector<uintptr_t> ExternalDumper::SignatureScan(std::string ida_sign, uintptr_t from, uintptr_t end, bool stop_first)
{
	Signature sign = Signature(ida_sign);
	return this->SignatureScan(sign, from, end);
}

std::vector<uintptr_t> ExternalDumper::SignatureScan(Signature sign, uintptr_t from, uintptr_t end, bool stop_first)
{
	auto res = std::vector<uintptr_t>();
	uintptr_t base = from == 0 ? this->p->base : from;
	end = end == 0 ? this->p->base + this->p->base_size : end;
	while (true)
	{
		auto region = GetMemoryRegion(base);

		// loop regions and read memory for each region and search for signature
		if (region.base == 0)
			break;

		auto buf = std::vector<BYTE>(region.size);
		if (!p->ReadMemory(region.base, region.size, buf.data()))
			break;

		for (size_t i = 0; i < region.size; i++)
		{
			uintptr_t addr = region.base + i;
			// check if addr reached end
			if (addr >= end)
				return res;
			// compare buf & signature
			if (sign.Compare(buf.data() + i))
			{
				res.push_back(addr);
				if (stop_first)
					return res;
			}
		}
		base += (uintptr_t)region.size;
	}
	
	return res;
}
