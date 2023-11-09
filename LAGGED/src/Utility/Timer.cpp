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

float LAG::Utility::Timer::GetMilliseconds()
{
	m_DeltaTime = TimerClock::now() - m_StartTime;
	return m_DeltaTime.count() * 1000.f;
}

float LAG::Utility::Timer::MarkSeconds()
{
	float mark = GetSeconds();
	ResetTimer();
	return mark;
}

float LAG::Utility::Timer::MarkMilliseconds()
{
	float mark = GetMilliseconds();
	ResetTimer();
	return mark;
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
