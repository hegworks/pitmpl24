#pragma once

namespace Uknitty
{

class CountdownTimer
{
public:
	CountdownTimer(float durationSeconds)
	{
		durationSeconds *= 10.0f;
		m_duration = durationSeconds;
		m_remaining = durationSeconds;
	}

	void Update(float deltaTime)
	{
		m_remaining -= deltaTime;
	}

	void Reset()
	{
		m_remaining = m_duration;
	}

	bool IsFinished() const
	{
		return m_remaining <= 0;
	}

	float GetRemaining() const
	{
		return m_remaining;
	}

private:
	float m_duration;
	float m_remaining;
};

} // namespace Uknitty
