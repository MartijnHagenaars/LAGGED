#pragma once
#include <chrono>
#include <string>
#include <unordered_map>
#include <deque>

#define LAG_PROFILE() LAG::ScopedProfiler INTERNAL_SCOPED_PROFILER(__FUNCTION__)

namespace LAG
{
	class Profiler
	{
		struct ProfilerRecord;
		friend class ScopedProfiler;
	public:
		static const std::unordered_map < std::string, ProfilerRecord>& GetProfilerRecords();

		// FIXME: ImGui code should NOT be handled here, 
		// It's fine for now, but needs to be cleaned up soon.
		static void OnImGui();
	private:
		static void StartScopedProfiler(const std::string& lookup);
		static void EndScopedProfiler(const std::string& lookup);

	private:
		using timePoint = std::chrono::time_point<std::chrono::steady_clock>;
		using timeSpan = std::chrono::nanoseconds;

		struct ProfilerRecord
		{
			timePoint	timeStart;
			timePoint	timeEnd;
			timeSpan	timeSpan;
			float		average = 0.0f;

			std::deque<float> history;
		};

		static inline std::unordered_map < std::string, ProfilerRecord> m_ProfilerRecords;
	};

	class ScopedProfiler
	{
	public:
		ScopedProfiler(const std::string& name);
		~ScopedProfiler();
	private:
		const std::string m_Name;
	};
}