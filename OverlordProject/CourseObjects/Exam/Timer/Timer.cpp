#include "stdafx.h"
#include "Timer.h"


Timer::Timer(float time)
	:m_Timer(time)
{
}


Timer::~Timer()
{
}

bool Timer::UpdateTimer(float delta)
{
	m_Timer -= delta;
	if (m_Timer <= 0.0f) return true;
	
	
	return false;
}

const float Timer::GetTime() const
{
	return m_Timer;
}
