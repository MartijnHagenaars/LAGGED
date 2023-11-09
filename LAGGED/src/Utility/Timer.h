#pragma once
#include <chrono>

namespace LAG::Utility
{
	using TimerClock = std::chrono::high_resolution_clock;
	class Timer
	{
	public:
		Timer();
		~Timer();

		float GetMilliseconds();
		float GetSeconds();

		//Marks the time in seconds
		float MarkSeconds();
		float MarkMilliseconds();

		void SetTimeScale(float timeScale);

		void ResetTimer();

	private:
		TimerClock::time_point m_StartTime;
		std::chrono::duration<float> m_DeltaTime;
		float m_TimeScale = 1.f;
	};
}
