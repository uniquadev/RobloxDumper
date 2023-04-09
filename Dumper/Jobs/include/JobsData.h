#pragma once

#include <stdint.h>

namespace Dumper::JobsData
{
	inline uintptr_t modulescriptinternal_xref;
	
	inline uintptr_t vmload_start;
	inline uintptr_t vmload_end;

	inline uintptr_t pseudo2addr_start;
	inline uintptr_t pseudo2addr_end;

	inline uintptr_t os_date_start;
	inline uintptr_t os_date_end;

	inline uintptr_t startscript_start;
	inline uintptr_t startscript_end;
	inline uintptr_t getstate;
}