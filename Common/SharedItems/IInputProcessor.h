#pragma once

#include "IInput.h"
#include "IInputKey.h"

namespace Uknitty
{

class IInputProcessor
{
public:
	IInputProcessor() = default;
	~IInputProcessor() = default;

	virtual void MouseCallback(double xPos, double yPos) = 0;
	virtual void KeyDown(Key key) = 0;
	virtual void KeyUp(Key key) = 0;
	virtual void ProcessInput(IKeyboard* iKeyboard) = 0;

private:

};

}
