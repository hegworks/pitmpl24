#pragma once
#include <IInput.h>
#include <string>

class RaspKeyboard : public IKeyboard
{
public:
	RaspKeyboard();
	virtual bool GetKey(Key key) const;

private:
	pthread_t m_keyboardThread;
	bool* m_keyDown;
	std::string m_keyboardLocation;

	void FindKeyboardLocation();
	std::string FindActiveKeyboardEv();

	static void* ProcessKeyboardThread(void* arg);
};
