// 12.5.2010 Guillaume

#include "stdafx.h"
#include "Timer.h"

LONGLONG performanceCounter()
{
	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);
	return current.QuadPart;
}

Timer::Timer()
{
	m_last = 0;
	m_delta = 1;
	m_deltad = 1;
	m_timeSinceStart = 0;

	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	m_invFrequency = 1.0 / (double)frequency.QuadPart;
}

Timer::~Timer()
{

}

double Timer::Tick()
{
	if (m_last == 0)
	{
		Restart();
	}

	LONGLONG now = performanceCounter();
	LONGLONG delta = now - m_last;
	m_last = now;

	m_deltad = ((double)delta) * m_invFrequency;
	m_delta = (float)m_deltad;
	m_timeSinceStart += m_deltad;

	return m_delta;
}

void Timer::Restart()
{
	m_last = performanceCounter();
	m_timeSinceStart = 0;
	m_delta = 0;
	m_deltad = 0;
}

TimeAverage::TimeAverage()
{
	m_index = 0;
	for (unsigned int i = 0; i < _countof(m_times); i++)
		m_times[i] = 1.0f / 60.0f;
}

void TimeAverage::NotifyNewDelta(float d)
{
	m_times[m_index] = d;
	m_index = (m_index + 1) % _countof(m_times);
}

float TimeAverage::ComputeAverage() const
{
	float average = 0;
	for (unsigned int i = 0; i < _countof(m_times); i++)
	{
		average += m_times[i];
	}
	return average / _countof(m_times);
}

//////////////////////////////////////////////////////////////////////////
void GameTimer::Tick()
{
	m_timer.Tick();
	m_timeSinceStart += m_timer.GetDelta() * m_speed;
	m_delta = m_timer.GetDelta() * m_speed;
}

void GameTimer::Restart()
{
	m_timeSinceStart = 0;
	m_timer.Restart();
}

GameTimer::GameTimer()
{
	m_speed = 1;
	Restart();
}
