#pragma once


class IMouse;
class IKeyboard;

class Input
{
public:
	Input(IKeyboard* keyboard, IMouse* const mouse);
	~Input();

	IKeyboard* GetKeyboard();
	IMouse* GetMouse() const;

private:
	IMouse* const mouse;
	IKeyboard* keyboard;
};

