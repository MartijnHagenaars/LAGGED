#include "Profiler.h"

// FIXME: PROFILER.CPP SHOULD NOT CONTAIN ANY IMGUI CODE.
// NEEDS TO BE (RE)MOVED AT SOME POINT. 
#include <ImGui/imgui.h>

namespace LAG
{
	const std::unordered_map<std::string, Profiler::ProfilerRecord>& Profiler::GetProfilerRecords()
	{
		return m_ProfilerRecords;
	}
	void Profiler::StartScopedProfiler(const std::string& lookup)
	{
		m_ProfilerRecords[lookup].timeStart = std::chrono::high_resolution_clock::now();
	}

	void Profiler::EndScopedProfiler(const std::string& lookup)
	{
		auto& entry = m_ProfilerRecords[lookup];

		entry.timeEnd = std::chrono::high_resolution_clock::now();
		entry.timeSpan += entry.timeEnd - entry.timeStart;
	}

	void Profiler::OnImGui()
	{
		ImGui::Begin("Profiler");

		for (auto& it : m_ProfilerRecords)
		{
			ProfilerRecord& rec = it.second;


			//float timeDuration = static_cast<float>((static_cast<double>(rec.timeSpan.count()) / 1'000'000.f));
			float timeDuration = std::chrono::duration<float, std::milli>(rec.timeSpan).count();

			rec.history.push_back(timeDuration);
			if (rec.history.size() > 100)
				rec.history.pop_front();

			rec.average = 0.f;
			for (const float val : rec.history)
				rec.average += val;

			rec.average = rec.average / static_cast<float>(rec.history.size());
		}

		for (auto& it : m_ProfilerRecords)
		{
			ProfilerRecord& rec = it.second;
			ImGui::LabelText(it.first.c_str(), "%fms, %i samples", rec.average, rec.history.size());
		}

		ImGui::End();

		for (auto& it : m_ProfilerRecords)
		{
			it.second.timeSpan = std::chrono::nanoseconds::zero();
		}
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