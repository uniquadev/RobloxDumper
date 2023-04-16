#include "Jobs.h"
#include "Memory.h"
#include "MemoryUtils.h"
#include "JobsData.h"
#include "PtrDeobf.h"
using namespace Dumper;
using namespace Memory;

using JobsData::ts_version_check_start;

bool dump_tsnext(JobsHandler* h)
{
	// 25
	auto offsets = get_immeoffsets(
		SearchSettings(ts_version_check_start, ts_version_check_start + 25, PAGE_EXECUTE_READ, false, false)
	);

	for (const auto& offset : offsets)
	{
		if (offset == 20 || offset == 24)
			continue;
		h->push_offset("TS_NEXT", offset);
		return true;
	}

	return false;
}