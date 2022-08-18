#include "Signature.h"
#include <iostream>

BYTE HexToInt(std::string hex)
{
	return (BYTE)std::stoul(hex, nullptr, 16);
}

Signature::Signature(std::string ida_sign)
{
	// storage
	this->sign = std::vector<BYTE>();
	this->mask = std::vector<char>();
	
	// split string with spaces
	std::vector<std::string> tokens;
	std::string token = "";
	
	// foreach ida_sing
	for (size_t i = 0; i < ida_sign.length(); i++)
	{
		char chr = ida_sign[i];
		if (chr == ' ')
		{
			tokens.push_back(token);
			token.clear();
		}
		else
		{
			token += chr;
		}
	}
	tokens.push_back(token); // push back last token

	// foreach and sanitize token
	for (size_t i = 0; i < tokens.size(); i++)
	{
		token = tokens[i];
		if (token == "?" || token == "??")
		{
			this->mask.push_back('?');
			this->sign.push_back(' ');
		}
		else
		{
			if (token.size() > 2)
				throw std::exception("wrong signature format");
			
			this->mask.push_back('x');
			this->sign.push_back(HexToInt(token));
		}
	}
	tokens.clear();
}

bool Signature::Compare(void* buffer)
{
	BYTE* buf = (BYTE*)buffer;
	for (size_t i = 0; i < this->sign.size(); i++)
	{
		if (this->mask[i] == 'x')
		{
			if (this->sign[i] != buf[i])
				return false;
		}
	}
	return true;
}
