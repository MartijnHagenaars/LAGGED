#pragma once
#include <chrono>

namespace LAG::Utility
{
	using TimerClock = std::chrono::system_clock;
	class Timer
	{
	public:
		Timer();
		~Timer();

		float GetMs();
		float GetSeconds();

		//Marks the time in Ms
		float Mark();

		void SetTimeScale(float timeScale);

		void ResetTimer();

	private:
		TimerClock::time_point m_StartTime;
		std::chrono::duration<float> m_DeltaTime;
		float m_TimeScale = 1.f;
	};
}
