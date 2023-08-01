// This file is part of the uniquadev/RobloxDumper and is licensed under MIT License; see LICENSE.txt for details

#include <windows.h>
#include <winuser.h>
#include <fstream>

#include "Memory.h"
#include "JobsHandler.h"

#define TITLE L"RoDumper"

int WINAPI main_thread(HMODULE hThread)
{
	Dumper::Memory::init(); // setup memory library
	
	auto jh = new Dumper::JobsHandler;
	jh->register_jobs();

	bool success = jh->run();

	auto dir = Dumper::Memory::get_dll_dir(hThread);
	std::ofstream ofs(dir / "dump.json");

	jh->format();
	ofs << jh->output.dump(4);
	ofs.close();

	delete jh;
	
	if (success)
		MessageBox(
			NULL,
			(LPCWSTR)L"Dump successfully completed!",
			(LPCWSTR)TITLE,
			MB_ICONINFORMATION | MB_OK
		);
	else
		MessageBox(
			NULL,
			(LPCWSTR)L"Dump failed!",
			(LPCWSTR)TITLE,
			MB_ICONERROR | MB_OK
		);

	FreeLibraryAndExitThread(hThread, 0);
	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
						DWORD  ul_reason_for_call,
						LPVOID lpReserved
						)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		HANDLE h = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)main_thread, hModule, 0, nullptr);
		if (h)
			CloseHandle(h);
	}
	return TRUE;
}

