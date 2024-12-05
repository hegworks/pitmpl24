#include <windows.h>

#include "Common.h"
#include "Game.h"
#include "WindowsGraphics.h"
#include <glad/glad.h>
#include <iostream>

void WindowsGraphics::SwapBuffer()
{
	glFlush();
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

GLFWwindow& WindowsGraphics::Window() const
{
	return *m_window;
}

WindowsGraphics::WindowsGraphics()
{
#pragma region Set console size and position
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	AllocConsole();
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.X = 700;
	coninfo.dwSize.Y = 9999;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);
	SetWindowPos(GetConsoleWindow(), HWND_TOP, 0, 0, 700, 1000, 0);
#pragma endregion Set console size and position

	// Initialize GLFW and set window properties.
	glfwInit();
	glfwWindowHint(GL_DEPTH_BUFFER_BIT, 16);
	glfwWindowHint(GL_DEPTH_BITS, 16);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // we need a base OpenGL 3.3 to emulate ES, otherwise use 3.1 for ES
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // for normal opengl
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

	// Creates the window.
	m_window = glfwCreateWindow(SCRWIDTH, SCRHEIGHT, "PC Based OpenGLES", NULL, NULL);

	// Error handling for if window creation failed.
	if(m_window == NULL)
	{
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
	}

	// Set the window to be the current context.
	glfwMakeContextCurrent(m_window);
	glfwSetWindowPos(m_window, 700, 100);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Error handling for if GLAD failed to initialize.
	if(!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!" << std::endl;
	}
}

void WindowsGraphics::Quit()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}
