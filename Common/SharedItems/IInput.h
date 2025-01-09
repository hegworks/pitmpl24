#pragma once
// group these 2 togeter

#include <functional> 
#include <glm/vec2.hpp>

#include "IInputKey.h"

class IMouse
{

public:
	virtual ~IMouse() = default;

	virtual bool GetButtonDown(MouseButton button) const = 0;
	virtual glm::vec2 GetPosition() = 0;
	virtual float GetScrollDelta() const = 0;

	void CaptureMouseInput()
	{
		if(!m_isCapturingMouseInput)
		{
			m_isCapturingMouseInput = true;
			OnCaptureMouseInput();
		}
	}

	void ReleaseMouseInput()
	{
		if(m_isCapturingMouseInput)
		{
			m_isCapturingMouseInput = false;
			OnReleaseMouseInput();
		}
	}

	bool IsCapturingMouseInput() const { return m_isCapturingMouseInput; }

protected:
	virtual void OnCaptureMouseInput() = 0;
	virtual void OnReleaseMouseInput() = 0;

	bool m_isCapturingMouseInput = false;
};

class IKeyboard
{
public:
	virtual bool GetKey(Key key) const = 0;
	virtual ~IKeyboard() = default;

	using KeyCallback = std::function<void(Key key, KeyAction action)>;
	virtual void SetKeyCallback(const KeyCallback& callback) = 0;
};
