#include "Input.h"

#include "IInput.h"

Input::Input(IKeyboard* keyboard, IMouse* const mouse) :
	mouse(mouse),
	keyboard(keyboard)
{
}

Input::~Input()
{
	delete(mouse);
	delete(keyboard);
}

IKeyboard* Input::GetKeyboard()
{
	return keyboard;
}

IMouse* Input::GetMouse() const
{
	return mouse;
}
