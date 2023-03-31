#include "Memory.h"

#include <stdexcept>
#include <Psapi.h>

void Dumper::Memory::init()
{
    r_procid = GetCurrentProcessId();
    r_process = GetCurrentProcess();
    r_module = GetModuleHandleA(NULL);
    LPMODULEINFO r_info = new MODULEINFO();
    if (GetModuleInformation(r_process, r_module, r_info, sizeof(MODULEINFO)) != 0)
        r_size = r_info->SizeOfImage;
    else
        throw std::runtime_error("Unable to retrive Roblox's module information");

    delete r_info;
}

std::filesystem::path Dumper::Memory::get_dll_dir(HMODULE dllModule)
{
    wchar_t Path[MAX_PATH];
    GetModuleFileNameW(dllModule, Path, MAX_PATH);
    return std::filesystem::path(std::wstring(Path)).parent_path();
}
