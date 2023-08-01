// This file is part of the uniquadev/RobloxDumper and is licensed under MIT License; see LICENSE.txt for details

#pragma once

#include <string>
#include <vector>
#include <iostream>

typedef unsigned char BYTE;

namespace Dumper::Memory
{
	_inline BYTE HexToInt(std::string hex)
	{
		return (BYTE)std::stoul(hex, nullptr, 16);
	}

	class Signature
	{
	public:
		std::vector<BYTE> sign;
		std::vector<char> mask;

		bool compare(void* buffer)
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
		
		void set(size_t idx, BYTE b)
		{
			this->sign[idx] = b;
			this->mask[idx] = 'x';
		}
		void unset(size_t idx)
		{
			this->sign[idx] = ' ';
			this->mask[idx] = '?';
		}

		Signature(std::vector<BYTE> s, std::vector<char> m)
		{
			sign = s;
			mask = m;
		}

		Signature(std::string ida_sign)
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
				if (token == "?" || token == "??") // unknown bytes case
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
	};
}