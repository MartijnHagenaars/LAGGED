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
		friend class ScopedProfiler;
	public:
		struct ProfilerRecord;

		static const std::unordered_map<std::string, ProfilerRecord>& GetRecords();

		static void CalculateAverages();

	public:
		using timePoint = std::chrono::time_point<std::chrono::steady_clock>;
		using timeSpan = std::chrono::nanoseconds;

		struct ProfilerRecord
		{
			timePoint timeStart;
			timePoint timeEnd;
			timeSpan timeSpan;
			float average = 0.0f;

			std::deque<float> history;
		};

	private:
		static void StartScopedProfiler(const std::string& lookup);
		static void EndScopedProfiler(const std::string& lookup);

		static inline std::unordered_map<std::string, ProfilerRecord> m_Records;
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