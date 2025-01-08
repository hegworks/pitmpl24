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
	if(button == MouseButton::LEFT)
	{
		std::cout << "Got mouse left key down on player\n";
	}
	if(button == MouseButton::RIGHT)
	{
		std::cout << "Got mouse right key down on player\n";
	}
	if(button == MouseButton::MIDDLE)
	{
		std::cout << "Got mouse middle key down on player\n";
	}

}

void PlayerCInput::OnMouseButtonUp(MouseButton button)
{
	if(button == MouseButton::LEFT)
	{
		std::cout << "Got mouse left key up on player\n";
	}
	if(button == MouseButton::RIGHT)
	{
		std::cout << "Got mouse right key up on player\n";
	}
	if(button == MouseButton::MIDDLE)
	{
		std::cout << "Got mouse middle key up on player\n";
	}
}

