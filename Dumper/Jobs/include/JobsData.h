// This file is part of the uniquadev/RobloxDumper and is licensed under MIT License; see LICENSE.txt for details

#pragma once

#include <stdint.h>

namespace Dumper::JobsData
{
	inline uintptr_t modulescriptinternal_xref;

	inline uintptr_t typename_start;
	
	inline uintptr_t vmload_start;
	inline uintptr_t vmload_end;

	inline uintptr_t pseudo2addr_start;
	inline uintptr_t pseudo2addr_end;

	inline uintptr_t os_date_start;
	inline uintptr_t os_date_end;

	inline uintptr_t luaopen_base_start;
	inline uintptr_t luaopen_base_end;

	inline uintptr_t ts_version_check_start;

	inline uintptr_t startscript_start;
	inline uintptr_t startscript_end;
	inline uintptr_t getstate;
}