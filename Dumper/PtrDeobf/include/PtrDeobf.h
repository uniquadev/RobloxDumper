#pragma once
#include <cstdint>
#include <string>

enum PtrObfuscationType
{
	ADD,
	SUB_P_X,
	SUB_X_P,
	XOR,
	NONE
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