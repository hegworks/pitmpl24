#include "RaspKeyboard.h"

#include "IInput.h"
#include <fstream>
#include <iostream>
#include <linux/input.h>
#include <sstream>
RaspKeyboard::RaspKeyboard() : m_keyDown{new bool[256] { false }}
{
	FindKeyboardLocation();
	pthread_create(&m_keyboardThread, nullptr, &ProcessKeyboardThread, this);
}

bool RaspKeyboard::GetKey(Key key) const
{
	switch(key)
	{
		case Key::A: return m_keyDown[KEY_A];
		case Key::B: return m_keyDown[KEY_B];
		case Key::C: return m_keyDown[KEY_C];
		case Key::D: return m_keyDown[KEY_D];
		case Key::E: return m_keyDown[KEY_E];
		case Key::F: return m_keyDown[KEY_F];
		case Key::G: return m_keyDown[KEY_G];
		case Key::H: return m_keyDown[KEY_H];
		case Key::I: return m_keyDown[KEY_I];
		case Key::J: return m_keyDown[KEY_J];
		case Key::K: return m_keyDown[KEY_K];
		case Key::L: return m_keyDown[KEY_L];
		case Key::M: return m_keyDown[KEY_M];
		case Key::N: return m_keyDown[KEY_N];
		case Key::O: return m_keyDown[KEY_O];
		case Key::P: return m_keyDown[KEY_P];
		case Key::Q: return m_keyDown[KEY_Q];
		case Key::R: return m_keyDown[KEY_R];
		case Key::S: return m_keyDown[KEY_S];
		case Key::T: return m_keyDown[KEY_T];
		case Key::U: return m_keyDown[KEY_U];
		case Key::V: return m_keyDown[KEY_V];
		case Key::W: return m_keyDown[KEY_W];
		case Key::X: return m_keyDown[KEY_X];
		case Key::Y: return m_keyDown[KEY_Y];
		case Key::Z: return m_keyDown[KEY_Z];
		case Key::NUM_0: return m_keyDown[KEY_0];
		case Key::NUM_1: return m_keyDown[KEY_1];
		case Key::NUM_2: return m_keyDown[KEY_2];
		case Key::NUM_3: return m_keyDown[KEY_3];
		case Key::NUM_4: return m_keyDown[KEY_4];
		case Key::NUM_5: return m_keyDown[KEY_5];
		case Key::NUM_6: return m_keyDown[KEY_6];
		case Key::NUM_7: return m_keyDown[KEY_7];
		case Key::NUM_8: return m_keyDown[KEY_8];
		case Key::NUM_9: return m_keyDown[KEY_9];
		case Key::TAB: return m_keyDown[KEY_TAB];
		case Key::CAPS_LOCK: return m_keyDown[KEY_CAPSLOCK];
		case Key::SHIFT_LEFT: return m_keyDown[KEY_LEFTSHIFT];
		case Key::CTRL_LEFT: return m_keyDown[KEY_LEFTCTRL];
		case Key::ALT_LEFT: return m_keyDown[KEY_LEFTALT];
		case Key::ESCAPE: return m_keyDown[KEY_ESC] || m_keyDown[KEY_HOMEPAGE];
		case Key::RIGHT_SHIFT: return m_keyDown[KEY_RIGHTSHIFT];
		case Key::ENTER: return m_keyDown[KEY_ENTER];
		case Key::ARROW_UP: return m_keyDown[KEY_UP];
		case Key::ARROW_RIGHT: return m_keyDown[KEY_RIGHT];
		case Key::ARROW_DOWN: return m_keyDown[KEY_DOWN];
		case Key::ARROW_LEFT: return m_keyDown[KEY_LEFT];
		case Key::SPACE: return m_keyDown[KEY_SPACE];
		default: throw std::runtime_error("Keycode not supported: " + std::to_string(static_cast<int>(key)));
	}
}

void RaspKeyboard::FindKeyboardLocation()
{
	const std::string ev{FindActiveKeyboardEv()};
	m_keyboardLocation = "/dev/input/" + ev;
}

std::string RaspKeyboard::FindActiveKeyboardEv()
{
	std::ifstream devicesFile;
	devicesFile.open("/proc/bus/input/devices");

	std::stringstream devicesStream;

	devicesStream << devicesFile.rdbuf();
	devicesFile.close();

	std::string devices = devicesStream.str();

	// The index in the file for our current new line position.
	std::size_t newLinePos{0};

	// The EV value for the current device.
	std::string ev{};

	// Moves every new line in devices file.
	do
	{
		// Checks for the Handlers field.
		// Always caches it, so we can use it if we find the correct device.
		if(devices.substr(newLinePos + 1, 11) == "H: Handlers")
		{
			// Index of the end of the line.
			std::size_t nextNewLine{devices.find('\n', newLinePos + 1) - 1};

			// The contents of the EV line.
			std::string line{devices.substr(newLinePos + 1, nextNewLine - (newLinePos + 1))};

			// The index of the last space, so we can get the last value.
			std::size_t lastSpace{line.rfind(' ')};

			// The very last value, which is the event.
			ev = line.substr(lastSpace + 1, line.length() - lastSpace + 1);
		}

		// Checks if current line start with the EV field.
		if(devices.substr(newLinePos + 1, 5) == "B: EV")
		{
			// The start of the value, while skipping the 'B: EV'.
			std::size_t start{newLinePos + 7};

			newLinePos = devices.find('\n', newLinePos + 1);

			// Grabs the string value of the EV field.
			std::string stringValue{devices.substr(start, newLinePos - start)};

			// Moves the hex string into an uint.
			unsigned int value;
			std::stringstream ss;
			ss << std::hex << stringValue;
			ss >> value;

			// Defines the mask for a keyboard.
			constexpr unsigned int keyboardMask{0x120013};

			if((value & keyboardMask) == keyboardMask)
			{
				break;
			}

			continue;
		}

		newLinePos = devices.find('\n', newLinePos + 1);
	} while(newLinePos != std::string::npos);

	return ev;
}

void* RaspKeyboard::ProcessKeyboardThread(void* arg)
{
	RaspKeyboard* input = static_cast<RaspKeyboard*>(arg);

	FILE* fKeyboard;
	fKeyboard = fopen(((RaspKeyboard*)arg)->m_keyboardLocation.c_str(), "r");

	if(fKeyboard == nullptr)
	{
		printf("Keyboard file not found.");
		pthread_exit(nullptr);
		return nullptr;
	}

	input_event event;

	while(true)
	{
		fread(&event, sizeof(input_event), 1, fKeyboard);

		if(event.type == (__u16)EV_KEY)
		{
			bool isPressed = event.value > 0;
			int key = event.code;

			auto it = input->m_keyStates.find(key);

			if(isPressed)
			{
				// If key is pressed but not already tracked
				if(it == input->m_keyStates.end())
				{
					input->m_keyStates[key] = true; // Track key
					if(input->m_keyCallback)
						input->m_keyCallback(CodeToKey(key), KeyAction::DOWN);
				}
			}
			else
			{
				// If key is released and was tracked
				if(it != input->m_keyStates.end())
				{
					input->m_keyStates.erase(it); // Remove key
					if(input->m_keyCallback)
						input->m_keyCallback(CodeToKey(key), KeyAction::UP);
				}
			}
		}
	}

	fclose(fKeyboard);
	pthread_exit(nullptr);
}

void RaspKeyboard::SetKeyCallback(const KeyCallback& callback)
{
	m_keyCallback = callback;
}

Key RaspKeyboard::CodeToKey(int code)
{
	switch(code)
	{
		case KEY_A: return Key::A;
		case KEY_B: return Key::B;
		case KEY_C: return Key::C;
		case KEY_D: return Key::D;
		case KEY_E: return Key::E;
		case KEY_F: return Key::F;
		case KEY_G: return Key::G;
		case KEY_H: return Key::H;
		case KEY_I: return Key::I;
		case KEY_J: return Key::J;
		case KEY_K: return Key::K;
		case KEY_L: return Key::L;
		case KEY_M: return Key::M;
		case KEY_N: return Key::N;
		case KEY_O: return Key::O;
		case KEY_P: return Key::P;
		case KEY_Q: return Key::Q;
		case KEY_R: return Key::R;
		case KEY_S: return Key::S;
		case KEY_T: return Key::T;
		case KEY_U: return Key::U;
		case KEY_V: return Key::V;
		case KEY_W: return Key::W;
		case KEY_X: return Key::X;
		case KEY_Y: return Key::Y;
		case KEY_Z: return Key::Z;
		case KEY_0: return Key::NUM_0;
		case KEY_1: return Key::NUM_1;
		case KEY_2: return Key::NUM_2;
		case KEY_3: return Key::NUM_3;
		case KEY_4: return Key::NUM_4;
		case KEY_5: return Key::NUM_5;
		case KEY_6: return Key::NUM_6;
		case KEY_7: return Key::NUM_7;
		case KEY_8: return Key::NUM_8;
		case KEY_9: return Key::NUM_9;
		case KEY_TAB: return Key::TAB;
		case KEY_CAPSLOCK: return Key::CAPS_LOCK;
		case KEY_LEFTSHIFT: return Key::SHIFT_LEFT;
		case KEY_LEFTCTRL: return Key::CTRL_LEFT;
		case KEY_LEFTALT: return Key::ALT_LEFT;
		case KEY_ESC: return Key::ESCAPE;
		case KEY_HOMEPAGE: return Key::ESCAPE;
		case KEY_RIGHTSHIFT: return Key::RIGHT_SHIFT;
		case KEY_ENTER: return Key::ENTER;
		case KEY_UP: return Key::ARROW_UP;
		case KEY_RIGHT: return Key::ARROW_RIGHT;
		case KEY_DOWN: return Key::ARROW_DOWN;
		case KEY_LEFT: return Key::ARROW_LEFT;
		case KEY_SPACE: return Key::SPACE;
		default: throw std::runtime_error("Keycode not supported: " + std::to_string(code));
	}
}
