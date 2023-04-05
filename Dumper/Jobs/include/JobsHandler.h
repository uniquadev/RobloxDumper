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
		std::vector<Job> children;
	};

	class JobsHandler
	{
	public:
		std::vector<Job> jobs = {};
		json output =
		{
			{"offsets", json::object()},
			{"addys", json::object()},
			{"subs", json::object()},
			{"errors", json::array()}
		};

		void push_addy(std::string name, uintptr_t addy)
		{
			output["addys"][name] = addy;
		}
		void push_offset(std::string name, uintptr_t offset)
		{
			output["offsets"][name] = offset;
		}
		void push_sub(std::string name, uintptr_t addy)
		{
			output["subs"][name] = json::object({
				{"address", addy}
			});
		}
		void push_error(std::string error)
		{
			output["errors"].push_back(error);
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

