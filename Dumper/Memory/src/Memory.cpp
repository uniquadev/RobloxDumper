#include "Memory.h"

#include <stdexcept>
#include <Psapi.h>

void Dumper::Memory::init()
{
    r_procid = GetCurrentProcessId();
    r_process = GetCurrentProcess();
    r_module = reinterpret_cast<uintptr_t>(GetModuleHandleA(NULL));
	
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

std::vector<MEMORY_BASIC_INFORMATION> Dumper::Memory::get_regions(uintptr_t addr, DWORD protect)
{
    auto res = std::vector<MEMORY_BASIC_INFORMATION>();
    MEMORY_BASIC_INFORMATION mbi;
    uintptr_t scan = addr == 0 ? r_module : addr;
    
    for (auto addr = scan; VirtualQuery((LPCVOID)addr, &mbi, sizeof(mbi)) != 0;
        addr = reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize)
    {
        if (mbi.State == MEM_COMMIT && (mbi.Protect & protect))
            res.push_back(mbi);
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
