#pragma once
#include "IInput.h"
#include "Input.h"
#include "WindowsInput.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

WindowsMouse::WindowsMouse(GLFWwindow& window) : m_window(window)
{
	glfwSetScrollCallback(&window, ScrollCallback);
}

bool WindowsMouse::GetButtonDown(MouseButtons button) const
{
	switch(button)
	{
		case MouseButtons::LEFT: return glfwGetMouseButton(&m_window, GLFW_MOUSE_BUTTON_LEFT);
		case MouseButtons::RIGHT: return glfwGetMouseButton(&m_window, GLFW_MOUSE_BUTTON_RIGHT);
		case MouseButtons::MIDDLE: return glfwGetMouseButton(&m_window, GLFW_MOUSE_BUTTON_MIDDLE);
		default:std::cout << "ERROR::INPUT::WINDOWS Mouse button not supported: " << static_cast<int>(button) << std::endl; return false;
	}
}

glm::vec2 WindowsMouse::GetPosition() const
{
	double xPosition, yPosition;
	glfwGetCursorPos(&m_window, &xPosition, &yPosition);
	return glm::vec2{xPosition, yPosition};
}

float WindowsMouse::GetScrollDelta() const
{
	return m_scrollDelta;
}

float WindowsMouse::m_scrollDelta{0.0f};

void WindowsMouse::ScrollCallback(GLFWwindow* /*window*/, double /*xOffset*/, double yOffset)
{
	m_scrollDelta = static_cast<float>(yOffset);
}


void WindowsKeyboard::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	WindowsKeyboard* keyboard = static_cast<WindowsKeyboard*>(glfwGetWindowUserPointer(window));
	if(keyboard->m_keyCallback)
	{
		keyboard->m_keyCallback(keyboard->GlfwKeyToEnumKey(key), keyboard->GlfwActionToEnumKeyAction(action));
	}
}

void WindowsKeyboard::SetKeyCallback(const KeyCallback& callback)
{
	m_keyCallback = callback;
}

WindowsKeyboard::WindowsKeyboard(GLFWwindow& window) : m_window(window) {
	glfwSetWindowUserPointer(&window, this);

	glfwSetKeyCallback(&window, KeyCallBack);
}

Key WindowsKeyboard::GlfwKeyToEnumKey(int glfwKey)
{
	switch(glfwKey)
	{
		case GLFW_KEY_A: return Key::A;
		case GLFW_KEY_B: return Key::B;
		case GLFW_KEY_C: return Key::C;
		case GLFW_KEY_D: return Key::D;
		case GLFW_KEY_E: return Key::E;
		case GLFW_KEY_F: return Key::F;
		case GLFW_KEY_G: return Key::G;
		case GLFW_KEY_H: return Key::H;
		case GLFW_KEY_I: return Key::I;
		case GLFW_KEY_J: return Key::J;
		case GLFW_KEY_K: return Key::K;
		case GLFW_KEY_L: return Key::L;
		case GLFW_KEY_M: return Key::M;
		case GLFW_KEY_N: return Key::N;
		case GLFW_KEY_O: return Key::O;
		case GLFW_KEY_P: return Key::P;
		case GLFW_KEY_Q: return Key::Q;
		case GLFW_KEY_R: return Key::R;
		case GLFW_KEY_S: return Key::S;
		case GLFW_KEY_T: return Key::T;
		case GLFW_KEY_U: return Key::U;
		case GLFW_KEY_V: return Key::V;
		case GLFW_KEY_W: return Key::W;
		case GLFW_KEY_X: return Key::X;
		case GLFW_KEY_Y: return Key::Y;
		case GLFW_KEY_Z: return Key::Z;
		case GLFW_KEY_0: return Key::NUM_0;
		case GLFW_KEY_1: return Key::NUM_1;
		case GLFW_KEY_2: return Key::NUM_2;
		case GLFW_KEY_3: return Key::NUM_3;
		case GLFW_KEY_4: return Key::NUM_4;
		case GLFW_KEY_5: return Key::NUM_5;
		case GLFW_KEY_6: return Key::NUM_6;
		case GLFW_KEY_7: return Key::NUM_7;
		case GLFW_KEY_8: return Key::NUM_8;
		case GLFW_KEY_9: return Key::NUM_9;
		case GLFW_KEY_TAB: return Key::TAB;
		case GLFW_KEY_CAPS_LOCK: return Key::CAPS_LOCK;
		case GLFW_KEY_LEFT_SHIFT: return Key::SHIFT_LEFT;
		case GLFW_KEY_LEFT_CONTROL: return Key::CTRL_LEFT;
		case GLFW_KEY_LEFT_ALT: return Key::ALT_LEFT;
		case GLFW_KEY_ESCAPE: return Key::ESCAPE;
		case GLFW_KEY_RIGHT_SHIFT: return Key::RIGHT_SHIFT;
		case GLFW_KEY_ENTER: return Key::ENTER;
		case GLFW_KEY_UP: return Key::ARROW_UP;
		case GLFW_KEY_RIGHT: return Key::ARROW_RIGHT;
		case GLFW_KEY_DOWN: return Key::ARROW_DOWN;
		case GLFW_KEY_LEFT: return Key::ARROW_LEFT;
		case GLFW_KEY_SPACE: return Key::SPACE;
		default: throw std::runtime_error("Key not supported");
	}
}

KeyAction WindowsKeyboard::GlfwActionToEnumKeyAction(int glfwKeyAction)
{
	switch(glfwKeyAction)
	{
		case GLFW_PRESS: return KeyAction::DOWN;
		case GLFW_RELEASE: return KeyAction::UP;
		default: throw std::runtime_error("Key action not supported");
	}
}

bool WindowsKeyboard::GetKey(Key key) const
{
	switch(key)
	{
		case Key::A: return glfwGetKey(&m_window, GLFW_KEY_A);
		case Key::B: return glfwGetKey(&m_window, GLFW_KEY_B);
		case Key::C: return glfwGetKey(&m_window, GLFW_KEY_C);
		case Key::D: return glfwGetKey(&m_window, GLFW_KEY_D);
		case Key::E: return glfwGetKey(&m_window, GLFW_KEY_E);
		case Key::F: return glfwGetKey(&m_window, GLFW_KEY_F);
		case Key::G: return glfwGetKey(&m_window, GLFW_KEY_G);
		case Key::H: return glfwGetKey(&m_window, GLFW_KEY_H);
		case Key::I: return glfwGetKey(&m_window, GLFW_KEY_I);
		case Key::J: return glfwGetKey(&m_window, GLFW_KEY_J);
		case Key::K: return glfwGetKey(&m_window, GLFW_KEY_K);
		case Key::L: return glfwGetKey(&m_window, GLFW_KEY_L);
		case Key::M: return glfwGetKey(&m_window, GLFW_KEY_M);
		case Key::N: return glfwGetKey(&m_window, GLFW_KEY_N);
		case Key::O: return glfwGetKey(&m_window, GLFW_KEY_O);
		case Key::P: return glfwGetKey(&m_window, GLFW_KEY_P);
		case Key::Q: return glfwGetKey(&m_window, GLFW_KEY_Q);
		case Key::R: return glfwGetKey(&m_window, GLFW_KEY_R);
		case Key::S: return glfwGetKey(&m_window, GLFW_KEY_S);
		case Key::T: return glfwGetKey(&m_window, GLFW_KEY_T);
		case Key::U: return glfwGetKey(&m_window, GLFW_KEY_U);
		case Key::V: return glfwGetKey(&m_window, GLFW_KEY_V);
		case Key::W: return glfwGetKey(&m_window, GLFW_KEY_W);
		case Key::X: return glfwGetKey(&m_window, GLFW_KEY_X);
		case Key::Y: return glfwGetKey(&m_window, GLFW_KEY_Y);
		case Key::Z: return glfwGetKey(&m_window, GLFW_KEY_Z);
		case Key::NUM_0: return glfwGetKey(&m_window, GLFW_KEY_0);
		case Key::NUM_1: return glfwGetKey(&m_window, GLFW_KEY_1);
		case Key::NUM_2: return glfwGetKey(&m_window, GLFW_KEY_2);
		case Key::NUM_3: return glfwGetKey(&m_window, GLFW_KEY_3);
		case Key::NUM_4: return glfwGetKey(&m_window, GLFW_KEY_4);
		case Key::NUM_5: return glfwGetKey(&m_window, GLFW_KEY_5);
		case Key::NUM_6: return glfwGetKey(&m_window, GLFW_KEY_6);
		case Key::NUM_7: return glfwGetKey(&m_window, GLFW_KEY_7);
		case Key::NUM_8: return glfwGetKey(&m_window, GLFW_KEY_8);
		case Key::NUM_9: return glfwGetKey(&m_window, GLFW_KEY_9);
		case Key::TAB: return glfwGetKey(&m_window, GLFW_KEY_TAB);
		case Key::CAPS_LOCK: return glfwGetKey(&m_window, GLFW_KEY_CAPS_LOCK);
		case Key::SHIFT_LEFT: return glfwGetKey(&m_window, GLFW_KEY_LEFT_SHIFT);
		case Key::CTRL_LEFT: return glfwGetKey(&m_window, GLFW_KEY_LEFT_CONTROL);
		case Key::ALT_LEFT:return glfwGetKey(&m_window, GLFW_KEY_LEFT_ALT);
		case Key::ESCAPE: return glfwGetKey(&m_window, GLFW_KEY_ESCAPE);
		case Key::RIGHT_SHIFT: return glfwGetKey(&m_window, GLFW_KEY_RIGHT_SHIFT);
		case Key::ENTER: return glfwGetKey(&m_window, GLFW_KEY_ENTER);
		case Key::ARROW_UP: return glfwGetKey(&m_window, GLFW_KEY_UP);
		case Key::ARROW_RIGHT: return glfwGetKey(&m_window, GLFW_KEY_RIGHT);
		case Key::ARROW_DOWN: return glfwGetKey(&m_window, GLFW_KEY_DOWN);
		case Key::ARROW_LEFT: return glfwGetKey(&m_window, GLFW_KEY_LEFT);
		case Key::SPACE: return glfwGetKey(&m_window, GLFW_KEY_SPACE);
		default: throw std::runtime_error("Key not supported");
	}
}
