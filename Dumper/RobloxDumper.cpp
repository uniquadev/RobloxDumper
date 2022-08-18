#include <iostream>
#include <fstream>
#include <filesystem>
#include <map>

#include "Process.h"
#include "Dumper.h"
#include "Signature.h"
#include "DumpJson.h"

#define DUMPINFO(message) std::cout << "[\x1B[36mINFO\033[0m] " << message << "\n"
#define DUMPERROR(message) std::cout << "[\x1B[31mERROR\033[0m] " << message << "\n"
#define DUMPSUCCESS(message) std::cout << "[\x1B[32mSUCCESS\033[0m] " << message << "\n"

ExternalDumper dumper = NULL;

void dump_printf()
{
	auto requestbudget = dumper.FindString("GetRequestBudgetForRequestType: Unknown request type.");
	if (requestbudget.size() < 1)
	{
		DUMPERROR("Could not find GetRequestBudgetForRequestType");
		return;
	}
	auto refs = dumper.GetRefs(requestbudget[0]);
	if (refs.size() < 1)
	{
		DUMPERROR("Could not find refs for requestbudget");
		return;
	}
	uintptr_t printf_addr = dumper.GetCalling(dumper.NextCall(refs[0]));
	subs["RBX::StandardOut::printf"] = dumper.GetOffset(printf_addr);
	DUMPSUCCESS("RBX::StandardOut::printf");
}

void dump_getdatamodel()
{
	auto contains = dumper.FindString("Contains '#'");
	if (contains.size() < 1)
	{
		DUMPERROR("Could not find Contains");
		return;
	}
	auto refs = dumper.GetRefs(contains[0]);
	if (refs.size() < 1)
	{
		DUMPERROR("Could not find refs for Contains");
		return;
	}
	auto datamodel_call = dumper.SignatureScan("E8 ? ? ? ? 85 C0", refs[0]);
	if (datamodel_call.size() < 1)
	{
		DUMPERROR("Could not find signature for getdatamodel call");
		return;
	}
	subs["RBX::DataModel::get"] = dumper.GetOffset(dumper.GetCalling(datamodel_call[0]));
	DUMPSUCCESS("RBX::DataModel::get");
}

void dump_simulationradius()
{
	auto world_step = dumper.FindString("worldStep - Adaptive");
	if (world_step.size() < 1)
	{
		DUMPERROR("Could not find worldStep");
		return;
	}
	auto refs = dumper.GetRefs(world_step[0]);
	if (refs.size() < 1)
	{
		DUMPERROR("Could not find refs for worldStep");
		return;
	}
	uintptr_t simulationradius_addr = dumper.GetFuncTop(refs[0]);
	if (simulationradius_addr == refs[0])
	{
		DUMPERROR("Could not find top of set_simulationradius");
		return;
	}
	subs["set_simulationradius"] = dumper.GetOffset(simulationradius_addr);
	DUMPSUCCESS("set_simulationradius");
}

void dump_grabscriptcontext()
{
	auto str = dumper.FindString("%d/%d/%d");
	if (str.size() < 1)
	{
		DUMPERROR("Could not find %d/%d/%d");
		return;
	}
	auto refs = dumper.GetRefs(str[0]);
	if (refs.size() < 1)
	{
		DUMPERROR("Could not find refs for %d/%d/%d");
		return;
	}
	auto func_top = dumper.GetFuncTop(refs[0]);
	if (func_top == refs[0])
	{
		DUMPERROR("Could not find top of grabscriptcontext caller");
		return;
	}
	auto call = dumper.SignatureScan("50 8D 8D ? ? ? ? E8 ? ? ? ?", func_top);
	uintptr_t grab_call = call[0] + 7;
	subs["grabscriptcontext"] = dumper.GetOffset(dumper.GetCalling(grab_call));
	DUMPSUCCESS("grabscriptcontext");
}

void dump_startscript()
{
	auto caller = dumper.SignatureScan("E8 ?? ?? ?? ?? 8D 8D ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B 9D ?? ?? ?? ?? 85 DB");
	if (caller.size() < 1)
	{
		DUMPERROR("Could not find signature for startscript caller");
		return;
	}
	auto start_script = dumper.GetCalling(caller[0]);
	subs["RBX::ScriptContext::startScript"] = dumper.GetOffset(start_script);
	DUMPSUCCESS("RBX::ScriptContext::startScript");
}

void dump_luaopenbase()
{
	auto _VERSION = dumper.SignatureScan("5F 56 45 52 C7");
	if (_VERSION.size() < 1)
	{
		DUMPERROR("Could not find signature of _VERSION usage");
		return;
	}
	auto luaopenbase = dumper.GetFuncTop(_VERSION[0]);
	subs["luaopen_base"] = dumper.GetOffset(luaopenbase);
	DUMPSUCCESS("luaopen_base");
}

void dump_luavmload()
{
	auto old_res = dumper.FindString("oldResult, moduleRef  = ...");
	if (old_res.size() < 1)
	{
		DUMPERROR("Could not find oldResult, moduleRef usage");
		return;
	}
	auto refs = dumper.GetRefs(old_res[0]);
	if (refs.size() < 1)
	{
		DUMPERROR("Could not find refs for oldResult, moduleRef usage");
		return;
	}
	/*
	*	Another way
	*	auto call_vmload = dumper.NextCall(refs[0], 1);
		subs["LuaVM::load"] = dumper.GetOffset(dumper.GetCalling(call_vmload));
	* 
	 */
	auto call_sign = dumper.SignatureScan("6A 00 83 C2 10 68 ? ? ? ? E8 ? ? ? ?", refs[0], refs[0]+200);
	if (call_sign.size() < 1)
	{
		DUMPERROR("Could not find signature for LuaVM::load call");
		return;
	}
	uintptr_t call = call_sign[0] + 10;
	uintptr_t vm_load = dumper.GetCalling(call);
	uintptr_t vm_load_end = vm_load + 1000;
	subs["LuaVM::load"] = dumper.GetOffset(vm_load);
	DUMPSUCCESS("LuaVM::load");
	// luau_load
	{ 
		call_sign = dumper.SignatureScan("2B C8 FF 75 0C 51 8B 4D EC 50 E8 ? ? ? ?", vm_load, vm_load_end);
		if (call_sign.size() < 1)
		{
			DUMPERROR("Could not find signature for lua_load call");
			return;
		}
		call = call_sign[0] + 10;
		subs["luau_load"] = dumper.GetOffset(dumper.GetCalling(call));
		DUMPSUCCESS("luau_load");
	}
	// ZSTD_decompress
	{
		call_sign = dumper.SignatureScan("E8 ? ? ? ? 83 C4 18 8B CB", vm_load, vm_load_end);
		if (call_sign.size() < 1)
		{
			DUMPERROR("Could not find signature for ZSTD_decompress call");
			return;
		}
		subs["ZSTD_decompress"] = dumper.GetOffset(dumper.GetCalling(call_sign[0]));
		DUMPSUCCESS("ZSTD_decompress");
	}
}

int main()
{
	std::cout << "Waiting Roblox\n";
	// get RobloxPlayerBeta.exe Process
	Process* p = NULL;
	do
		p = Process::ByName(L"RobloxPlayerBeta.exe");
	while (p == NULL);
	
	// get RobloxPlayerBeta.exe base
	auto path = p->GetPath();
	std::string version = path.parent_path().filename().string();
	
	// info
	DUMPINFO("Process base: " << std::hex << p->base);
	DUMPINFO("Process size: " << std::hex << p->base_size);
	// DUMPINFO("Process path: " << path);
	DUMPINFO("Roblox version: " << version << "\n");

	// new ExternalDumper
	dumper = ExternalDumper(p);
	
	// dumps
	dump_printf();
	dump_getdatamodel();
	dump_startscript();
	dump_simulationradius();
	dump_grabscriptcontext();
	dump_luavmload();
	dump_luaopenbase();
	

	// store dump results
	std::ofstream out("../out.json", std::ifstream::trunc);
	out << get_dump(version);
	out.close();
	
	delete p;
}