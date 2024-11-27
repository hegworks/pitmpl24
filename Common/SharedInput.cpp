#include "IInput.h"
#include "SharedInput.h"

SharedInput::SharedInput(IKeyboard* keyboard, IMouse* const mouse) :
	m_iMouse(mouse),
	m_iKeyboard(keyboard)
{
}

SharedInput::~SharedInput()
{
	delete(m_iMouse);
	delete(m_iKeyboard);
}

IKeyboard* SharedInput::GetKeyboard()
{
	return m_iKeyboard;
}

IMouse* SharedInput::GetMouse() const
{
	return m_iMouse;
}
