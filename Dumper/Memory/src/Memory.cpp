// This file is part of the uniquadev/RobloxDumper and is licensed under MIT License; see LICENSE.txt for details

#include "Memory.h"

#include <string>
#include <stdexcept>
#include <Psapi.h>
#include <dbghelp.h>

void Dumper::Memory::init()
{
    r_procid = GetCurrentProcessId();
    r_process = GetCurrentProcess();
    r_module = reinterpret_cast<uintptr_t>(GetModuleHandleA(NULL));
	
    PIMAGE_NT_HEADERS nt_header = ImageNtHeader((PVOID)r_module);
    WORD nsections = nt_header->FileHeader.NumberOfSections;
    PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(nt_header);
    size_t total_size = 0;

    // retrive sections location
    for (WORD i = 0; i < nsections; i++)
    {
        std::string name = (char*)&section->Name;
        if (name == ".text")
        {
            r_text_start = r_module + section->VirtualAddress;
            r_text_end = r_text_start + section->Misc.VirtualSize;
        }
        else if (name == ".rdata")
        {
            r_rdata_start = r_module + section->VirtualAddress;
            r_rdata_end = r_rdata_start + section->Misc.VirtualSize;
        }
        else if (name == ".data")
        {
            r_data_start = r_module + section->VirtualAddress;
            r_data_end = r_data_start + section->Misc.VirtualSize;
        }
        total_size += section->Misc.VirtualSize;
        section++;
    }
    r_roblox_end = r_module + total_size;

    size_t r_size;
    LPMODULEINFO r_info = new MODULEINFO();
    if (GetModuleInformation(r_process, (HMODULE)r_module, r_info, sizeof(MODULEINFO)) != 0)
        r_size = r_info->SizeOfImage;
    else
        throw std::runtime_error("Unable to retrive Roblox's module information");

    r_module_end = r_module + r_size;

    delete r_info;
}

std::filesystem::path Dumper::Memory::get_dll_dir(HMODULE dllModule)
{
    wchar_t Path[MAX_PATH];
    GetModuleFileNameW(dllModule, Path, MAX_PATH);
    return std::filesystem::path(std::wstring(Path)).parent_path();
}

std::vector<MEMORY_BASIC_INFORMATION> Dumper::Memory::get_regions(uintptr_t addr, DWORD protect, uintptr_t end_addr)
{
    auto res = std::vector<MEMORY_BASIC_INFORMATION>();
    MEMORY_BASIC_INFORMATION mbi;
    uintptr_t scan = addr == 0 ? r_module : addr;
    
    for (auto addr = scan; VirtualQuery((LPCVOID)addr, &mbi, sizeof(mbi)) != 0;
        addr = reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize)
    {
        if (end_addr && addr > end_addr)
            break;
        if (mbi.State == MEM_COMMIT && (mbi.Protect & protect))
            res.push_back(mbi);
    }
    
    if (res.size() > 0)
    {
        // adjust first region to start from addr
        if ((uintptr_t)res[0].BaseAddress < addr)
        {
            const auto region = res[0];
            uintptr_t base = (uintptr_t)region.BaseAddress;
            auto size = region.RegionSize;

            res[0].BaseAddress = (PVOID)addr;
            res[0].RegionSize = (base + size) - addr;
        }

        // adjust last region to end at the right end address
        const auto last = res.back();
        if (end_addr != 0 && ((uintptr_t)(last.BaseAddress) + last.RegionSize) > end_addr)
        {
            const auto region = res.back();
            uintptr_t base = (uintptr_t)region.BaseAddress;
            auto size = region.RegionSize;

            res.back().RegionSize = end_addr - base;
        }
    }
    

    return res;
}

std::optional<MEMORY_BASIC_INFORMATION> Dumper::Memory::get_region(uintptr_t addr)
{
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery((LPCVOID)addr, &mbi, sizeof(mbi)) != 0)
    {
        if (mbi.State == MEM_COMMIT)
            return mbi;
    }
    return std::nullopt;
}
