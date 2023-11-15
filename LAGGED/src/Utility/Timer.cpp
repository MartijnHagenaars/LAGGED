#include "Precomp.h"
#include "Timer.h"

LAG::Timer::Timer()
{
	ResetTimer();
	m_TimeScale = 1.f;
	m_DeltaTime = std::chrono::duration<float>(0.f);
}

LAG::Timer::~Timer()
{

}

float LAG::Timer::GetSeconds()
{
	m_DeltaTime = TimerClock::now() - m_StartTime;
	return m_DeltaTime.count();
}

float LAG::Timer::GetMilliseconds()
{
	m_DeltaTime = TimerClock::now() - m_StartTime;
	return m_DeltaTime.count() * 1000.f;
}

float LAG::Timer::MarkSeconds()
{
	float mark = GetSeconds();
	ResetTimer();
	return mark;
}

float LAG::Timer::MarkMilliseconds()
{
	float mark = GetMilliseconds();
	ResetTimer();
	return mark;
}

void LAG::Timer::SetTimeScale(float timeScale)
{
	if (timeScale > 0.f)
		m_TimeScale = timeScale;
}

void LAG::Timer::ResetTimer()
{
	m_StartTime = TimerClock::now();
}
