#pragma once
#include "json.hpp"

using json = nlohmann::json;

json subs = {};
json offsets = {};

std::string get_dump(std::string version)
{
	json j = {
		{"version", version},
		{"subs", subs},
		{"offsets", offsets}
	};
	return j.dump();
}