#include "Profiler.h"

namespace LAG
{
	const size_t MAX_PROFILE_HISTORY = 64;

	const std::unordered_map<std::string, Profiler::ProfilerRecord>& Profiler::GetRecords()
	{
		return m_Records;
	}

	void Profiler::CalculateAverages()
	{
		for (auto& it : m_Records)
		{
			ProfilerRecord& rec = it.second;

			rec.history.push_back(std::chrono::duration<float, std::milli>(rec.timeSpan).count());
			if (rec.history.size() > MAX_PROFILE_HISTORY)
				rec.history.pop_front();

			rec.average = 0.f;
			for (const float val : rec.history)
				rec.average += val;

			rec.average = rec.average / static_cast<float>(rec.history.size());
		}

		for (auto& it : m_Records)
		{
			it.second.timeSpan = std::chrono::nanoseconds::zero();
		}
	}

	void Profiler::StartScopedProfiler(const std::string& lookup)
	{
		m_Records[lookup].timeStart = std::chrono::high_resolution_clock::now();
	}

	void Profiler::EndScopedProfiler(const std::string& lookup)
	{
		auto& entry = m_Records[lookup];

		entry.timeEnd = std::chrono::high_resolution_clock::now();
		entry.timeSpan += entry.timeEnd - entry.timeStart;
	}


	/////////////////////
	// Scoped profiler //
	/////////////////////

	ScopedProfiler::ScopedProfiler(const std::string& name) :
		m_Name(name)
	{
		Profiler::StartScopedProfiler(m_Name);
	}

	ScopedProfiler::~ScopedProfiler()
	{
		Profiler::EndScopedProfiler(m_Name);
	}

}