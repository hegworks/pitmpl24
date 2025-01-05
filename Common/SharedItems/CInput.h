#pragma once

#include "IInput.h"
#include "IInputKey.h"

namespace Uknitty
{

class CInput
{
public:
	virtual void ProcessMousePosition([[maybe_unused]] double xPos, [[maybe_unused]] double yPos) {};
	virtual void ProcessKeyboard([[maybe_unused]] IKeyboard* iKeyboard) {};
	virtual void OnKeyDown([[maybe_unused]] Key key) {};
	virtual void OnKeyUp([[maybe_unused]] Key key) {};
};

} // namespace Uknitty
