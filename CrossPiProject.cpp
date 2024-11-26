// CrossPiProject.cpp : This file contains the PC 'main' function. Program execution begins and ends there.
//
#include "Game.h"
#include "SharedInput.h"
#include "WindowsGraphics.h"
#include "WindowsInput.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <iostream>

#ifdef WINDOWS_BUILD
// in case of a laptop with poor on board GPU, lets make sure we switch on any power gpu's we have to get max compatability
extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

extern "C"
{
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif


//
WindowsGraphics* WinGraphics;
SharedInput* sharedInput;
Game* game;

// Mains only purpose is to set up OGL and then jump to general game code
int main()
{
	WinGraphics = new WindowsGraphics();
	glfwSwapInterval(1); // stop the windows build updating without vblank so its the same speed as pi



	sharedInput = new SharedInput(new WindowsKeyboard(WinGraphics->Window()), new WindowsMouse(WinGraphics->Window()));
	printf("This cross project was partly inspired by BUas Student Ferri de Lange\n");
	printf("This GPU supplied by  :%s\n", glGetString(GL_VENDOR));
	printf("This GPU supports GL  :%s\n", glGetString(GL_VERSION));
	printf("This GPU Renders with :%s\n", glGetString(GL_RENDERER));
	printf("This GPU Shaders are  :%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	game = new Game(sharedInput, WinGraphics);

	game->Start();

	return 0;
}

void framebufferSizeCallback(GLFWwindow* /*window*/, int width, int height)
{
	glViewport(0, 0, width, height);
}
