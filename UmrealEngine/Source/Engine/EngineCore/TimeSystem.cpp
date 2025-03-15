#include "pch.h"

TimeSystem TimeSystem::Time;
TimeSystem& Time = TimeSystem::Time;

TimeSystem::TimeSystem()
{
	QueryPerformanceFrequency(&m_frequency);
	QueryPerformanceCounter(&m_previousTime);
	QueryPerformanceCounter(&m_currentTime);
}
TimeSystem::~TimeSystem() = default;

void TimeSystem::TimeSystemUpdate()
{
	++m_frameCount;

	m_previousTime = m_currentTime;
	QueryPerformanceCounter(&m_currentTime);
	LONGLONG tickTime = m_currentTime.QuadPart - m_previousTime.QuadPart;

	m_realtimeSinceStartup += tickTime;
	m_time += tickTime * timeScale;

	m_unscaledDeltaTime = double(tickTime) / double(m_frequency.QuadPart);
	m_deltaTime			= (std::min)(m_unscaledDeltaTime * timeScale, maximumDeltaTime);

	if (fixedTimeStep > std::numeric_limits<double>::epsilon())
	{
		m_elapsedFixedTime += m_deltaTime;
	}
}

bool TimeSystem::TimeSystemFixedUpdate()
{
	if (m_elapsedFixedTime >= fixedTimeStep && fixedTimeStep > std::numeric_limits<double>::epsilon())
	{
		m_fixedDeltaTime = m_elapsedFixedTime;
		m_fixedUnscaledDeltaTime = m_elapsedFixedTime / timeScale;
		m_elapsedFixedTime -= fixedTimeStep;
		return true;
	}
	return false;
}

void TimeSystem::Engine::TimeSystemUpdate()
{
	Time.TimeSystemUpdate();
}

bool TimeSystem::Engine::TimeSystemFixedUpdate()
{
	return Time.TimeSystemFixedUpdate();
}
