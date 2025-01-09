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

	virtual bool GetButtonDown(MouseButton button) const override;
	virtual glm::vec2 GetPosition() override;
	virtual float GetScrollDelta() const override;

protected:
	virtual void OnCaptureMouseInput() override;
	virtual void OnReleaseMouseInput() override;

private:
	GLFWwindow& m_window;

	static float m_scrollDelta;
	static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
};
