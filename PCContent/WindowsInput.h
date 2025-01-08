#pragma once
// 2 input classes
#include "IInput.h"
#include <GLFW/glfw3.h>

class WindowsKeyboard : public IKeyboard
{
public:
	WindowsKeyboard(GLFWwindow& window);
	virtual bool GetKey(Key key) const;

private:
	GLFWwindow& m_window;

	static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
	Key GlfwKeyToEnumKey(int glfwKey);
	KeyAction GlfwActionToEnumKeyAction(int glfwKeyAction);

	virtual void SetKeyCallback(const KeyCallback& callback);
	KeyCallback m_keyCallback;
};

class WindowsMouse : public IMouse
{
public:
	WindowsMouse(GLFWwindow& window);

	virtual bool GetButtonDown(MouseButton button) const;
	virtual glm::vec2 GetPosition();
	virtual float GetScrollDelta() const;

private:
	GLFWwindow& m_window;

	static float m_scrollDelta;
	static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
};
