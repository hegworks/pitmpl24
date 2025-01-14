#include "PlayerCInput.h"

#include "IInput.h"
#include "IInputKey.h"
#include <iostream>

void PlayerCInput::OnKeyDown(Key key)
{
	if(key == FORWARD_KEY) m_isForwardKeyDown = true;
	if(key == BACKWARD_KEY) m_isBackwardKeyDown = true;
	if(key == LEFT_KEY) m_isLeftKeyDown = true;
	if(key == RIGHT_KEY) m_isRightKeyDown = true;
}

void PlayerCInput::OnKeyUp(Key key)
{
	if(key == FORWARD_KEY) m_isForwardKeyDown = false;
	if(key == BACKWARD_KEY) m_isBackwardKeyDown = false;
	if(key == LEFT_KEY) m_isLeftKeyDown = false;
	if(key == RIGHT_KEY) m_isRightKeyDown = false;
}

void PlayerCInput::OnMouseButtonDown(MouseButton button)
{
	if(button == SHOOT_BUTTON)
	{
		if(m_onShootInputCallback)
		{
			m_onShootInputCallback();
		}
	}
}

void PlayerCInput::OnKeyUpAll()
{
	m_isForwardKeyDown = false;
	m_isBackwardKeyDown = false;
	m_isLeftKeyDown = false;
	m_isRightKeyDown = false;
}
