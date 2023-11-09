#include "Precomp.h"
#include "Timer.h"

LAG::Utility::Timer::Timer()
{
	ResetTimer();
	m_TimeScale = 1.f;
	m_DeltaTime = std::chrono::duration<float>(0.f);
}

LAG::Utility::Timer::~Timer()
{

}

float LAG::Utility::Timer::GetSeconds()
{
	m_DeltaTime = TimerClock::now() - m_StartTime;
	return m_DeltaTime.count();
}

float LAG::Utility::Timer::GetMs()
{
	m_DeltaTime = TimerClock::now() - m_StartTime;
	return m_DeltaTime.count() * 1000.f;
}

float LAG::Utility::Timer::Mark()
{
	float ms = GetMs();
	ResetTimer();
	return ms;
}

void LAG::Utility::Timer::SetTimeScale(float timeScale)
{
	if (timeScale > 0.f)
		m_TimeScale = timeScale;
}

void LAG::Utility::Timer::ResetTimer()
{
	m_StartTime = TimerClock::now();
}
