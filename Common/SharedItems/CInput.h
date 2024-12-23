#pragma once

#include "IInput.h"
#include "IInputKey.h"

namespace Uknitty
{

class CInput
{
public:
	virtual void ProcessMousePosition(double xPos, double yPos) {};
	virtual void ProcessKeyboard(IKeyboard* iKeyboard) {};
	virtual void OnKeyDown(Key key) {};
	virtual void OnKeyUp(Key key) {};
};

} // namespace Uknitty
