#pragma once
#include <cstdint>
#include <string>

enum PtrObfuscationType
{
	ADD,
	MIN_P_O,
	MIN_O_P,
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
		case MIN_P_O:
			return "MIN_P_O";
		case MIN_O_P:
			return "MIN_O_P";
		case XOR:
			return "XOR";
		default:
			return "NONE";
		}
	}

	PtrObfuscationType get_ptrobf_type(uintptr_t text_start, size_t length = 300);
	std::string get_ptrobf_type_str(uintptr_t text_start, size_t length = 300);
}