#pragma once
class IMouse;
class IKeyboard;

class SharedInput
{
public:
	SharedInput(IKeyboard* keyboard, IMouse* const mouse);
	~SharedInput();

	IKeyboard* GetKeyboard();
	IMouse* GetMouse() const;

private:
	IMouse* const m_iMouse = nullptr;
	IKeyboard* m_iKeyboard = nullptr;
};
