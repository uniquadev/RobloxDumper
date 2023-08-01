// This file is part of the uniquadev/RobloxDumper and is licensed under MIT License; see LICENSE.txt for details

#pragma once

#include <cstdint>
#include <string>

enum PtrObfuscationType
{
	ADD,		// (obj + off) + *(uintptr_t*)(obj + off)
	SUB_P_X,	// *(uintptr_t*)(obj + off) - (obj + off)
	SUB_X_P,	// (obj + off) - *(uintptr_t*)(obj + off)
	XOR,		// (obj + off) ^ *(uintptr_t*)(obj + off)
	NONE_TYPE	// *(uintptr_t*)(obj + off)
};

// SUB_P_X or SUB_X_P
enum PtrObfPosition
{
	LEFT,
	RIGHT,
	NONE_POSITION
};

namespace Dumper::PtrDeobf
{
	static std::string ptrobf_type_to_str(PtrObfuscationType type)
	{
		switch (type)
		{
		case ADD:
			return "ADD";
		case SUB_P_X:
			return "SUB_P_X";
		case SUB_X_P:
			return "SUB_X_P";
		case XOR:
			return "XOR";
		default:
			return "NONE";
		}
	}

	PtrObfuscationType get_ptrobf_type(uintptr_t text_start, size_t length = 300);
	std::string get_ptrobf_type_str(uintptr_t text_start, size_t length = 300);
}