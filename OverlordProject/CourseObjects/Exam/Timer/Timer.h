#pragma once
class Timer
{
public:
	Timer(float time);
	~Timer();
	bool UpdateTimer(const float delta);
	const float GetTime() const;

private:
	float m_Timer;
};

