#pragma once
// This is where most of the PC set up content is contained, its very similar to Jacco's tmpl8

#include "IGraphics.h"
#include <GLFW/glfw3.h>

class WindowsGraphics : public IGraphics
{
public:
	WindowsGraphics();

	virtual void Quit();
	virtual void SwapBuffer();
	GLFWwindow& Window() const;

private:
	GLFWwindow* m_window = nullptr;
};
