#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
#include "JobsData.h"
using namespace Dumper;
using namespace Memory;


/*
*	Another way could be searching for "no value" string and xref it to get inside typename
*/
using rlua_typename_cconv = const char* __fastcall(uintptr_t L, int tt);

bool dump_types(JobsHandler* h)
{
	static auto rlua_typename = (rlua_typename_cconv*)(JobsData::typename_start);
    try
    {
        std::vector<int> userdatas = {};
        int string = 0;
        for (int i = 0; i < 10; i++)
        {
            std::string type = rlua_typename(0, i);
            if (type == "nil")
                h->push_type("LUA_TNIL", i);
            else if (type == "boolean")
                h->push_type("LUA_TBOOLEAN", i);
            else if (type == "userdata")
                userdatas.push_back(i);
            else if (type == "number")
                h->push_type("LUA_TNUMBER", i);
            else if (type == "vector")
                h->push_type("LUA_TVECTOR", i);
            else if (type == "string")
            {
                string = i;
                h->push_type("LUA_TSTRING", i);
            }
            else if (type == "thread")
                h->push_type("LUA_TTHREAD", i);
            else if (type == "function")
                h->push_type("LUA_TFUNCTION", i);
            else if (type == "table")
                h->push_type("LUA_TTABLE", i);
        }
        if (userdatas[0] < string)
        {
            h->push_type("LUA_TLIGHTUSERDATA", userdatas[0]);
            h->push_type("LUA_TUSERDATA", userdatas[1]);
        }
        else
        {
            h->push_type("LUA_TLIGHTUSERDATA", userdatas[1]);
            h->push_type("LUA_TUSERDATA", userdatas[0]);
        }
    }
    catch (...)
    {
        JOBERROR(h, "Unexpected error during luau types dumping");
    }

	return true;
}