#pragma once
#include <IInput.h>

#include <string>
#include <unordered_map>

class RaspKeyboard : public IKeyboard
{
public:
	RaspKeyboard();
	virtual bool GetKey(Key key) const;

private:
	pthread_t m_keyboardThread;
	bool* m_keyDown = nullptr;
	std::string m_keyboardLocation;

	void FindKeyboardLocation();
	std::string FindActiveKeyboardEv();

	static void* ProcessKeyboardThread(void* arg);

	virtual void SetKeyCallback(const KeyCallback& callback);
	KeyCallback m_keyCallback;
	std::unordered_map<int, bool> m_keyStates;

	static Key CodeToKey(int code);
};
