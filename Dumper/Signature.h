#pragma once
#include <vector>
#include <string>
#include <Windows.h>

class Signature {
public:
	std::vector<char> mask;
	std::vector<BYTE> sign;
	// constructor
	Signature(std::string ida_sign);
	// methods
	bool Compare(void* buffer);
};