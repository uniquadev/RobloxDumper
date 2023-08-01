// This file is part of the uniquadev/RobloxDumper and is licensed under MIT License; see LICENSE.txt for details

#pragma once

#include <vector>
#include <functional>
#include <stdexcept>

#include "Json.hpp"

namespace Dumper
{
	using json = nlohmann::json;

	class DumpException : public std::runtime_error
	{
	public:
		explicit DumpException(const std::string& message) : std::runtime_error(message) {}
	};

	class JobsHandler;
	struct Job
	{
		std::string name;
		std::function<bool(JobsHandler*)> f;
		std::vector<Job> children;
	};

	class JobsHandler
	{
	public:
		size_t errors = 0;
		std::vector<Job> jobs = {};
		json output =
		{
			{"offsets", json::object()},
			{"addys", json::object()},
			{"subs", json::object()},
			{"ptrobf", json::object()},
			{"types", json::object()},
			{"errors", json::array()}
		};

		std::string int2hex(uintptr_t num) {
			std::stringstream stream;
			if (num & ((uintptr_t)INTPTR_MAX + 1)) { // sign bit
				stream << "-0x" << std::uppercase << std::hex << (~num) + 1;
			} else {
				stream << "0x" << std::uppercase << std::hex << num;
			}
			return stream.str();
		}
		void push_addy(std::string name, uintptr_t addy)
		{
			output["addys"][name] = json::object({
				{"address", addy},
				{"address_hex", int2hex(addy)},
			});
		}
		void push_offset(std::string name, int64_t offset)
		{
			output["offsets"][name] = {
				{"offset", offset},
				{"offset_hex", int2hex(offset)},
			};
		}
		void push_sub(std::string name, uintptr_t addy)
		{
			output["subs"][name] = json::object({
				{"address", addy},
				{"address_hex", int2hex(addy)},
			});
		}
		void push_ptrobf(std::string name, std::string obf)
		{
			output["ptrobf"].push_back(
				{name, obf}
			);
		}
		void push_type(std::string name, int type)
		{
			output["types"][name] = type;
		}
		void push_error(std::string error)
		{
			output["errors"].push_back(error);
			errors++;
		}

		bool run();
		void run_job(Job job);
		void format();

		void register_jobs();
		void register_job(std::string name, std::function<bool(JobsHandler*)> f, std::vector<Job> children = {})
		{
			jobs.push_back({ name, f, children });
		};
	};
}

