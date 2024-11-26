#include "IInput.h"
#include "SharedrInput.h"

SharedInput::SharedInput(IKeyboard* keyboard, IMouse* const mouse) :
	mouse(mouse),
	keyboard(keyboard)
{
}

SharedInput::~SharedInput()
{
	delete(mouse);
	delete(keyboard);
}

IKeyboard* SharedInput::GetKeyboard()
{
	return keyboard;
}

IMouse* SharedInput::GetMouse() const
{
	return mouse;
}
