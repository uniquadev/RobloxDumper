#pragma once

#include <vector>
#include <functional>
#include <stdexcept>

#include "Libs/Json.hpp"

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
	};

	class JobsHandler
	{
	public:
		std::vector<Job> jobs = {};
		json output =
		{
			{"CPUTime", 0},
			{"Offsets", json::object()},
			{"Addys", json::object()},
			{"Errors", json::array()}
		};

		void push_addy(std::string name, uintptr_t addy)
		{
			output["Addys"][name] = addy;
		}
		void push_offset(std::string name, uintptr_t offset)
		{
			output["Offsets"][name] = offset;
		}

		bool run()
		{
			std::clock_t started_at = std::clock();

			for (const auto& job : jobs)
			{
				if (!job.f(this))	// if job return false then force stop dumping
				{
					output["Errors"].push_back("STOPPED AT " + job.name);
					return false;
				}
			}

			std::stringstream stream2;
			stream2 << 1000.0 * (std::clock() - started_at) / CLOCKS_PER_SEC << " ms";
			output["CPUTime"] = stream2.str();

			return true;
		}

		/// <summary>
		/// Convert uintptr_t values to hex strings inside a json table. (beautification purpose)
		/// </summary>
		void int2hex_json(json& table)
		{
			for (auto& [key, val] : table.items())
			{
				auto offset = val.get<uintptr_t>();
				std::stringstream stream;
				stream << "0x" << std::hex << offset;
				table[key] = stream.str();
			}
		}

		void format()
		{
			int2hex_json(output["Offsets"]);
			int2hex_json(output["Addys"]);
		}

		void register_jobs();
	};
}

