#pragma once

#ifdef WINDOWS_BUILD
#include <GLFW/glfw3.h>

#else
#include <GLES2/gl2.h>

#endif

#include "IInputKey.h"
#include <vector>
#include <unordered_set>

struct ImFont;

namespace Uknitty
{
class CameraObject;
class Interfaces;
class Input;
class Render;
class Flow;
class FlowInput;
class FlowInputRender;
class Engine;
}

class SharedInput;
class IGraphics;
class IMouse;
class IKeyboard;

class Player;
class SceneManager;
class GameplayEvents;
class GameManager;

class Game
{
public:
	Game(SharedInput* input, IGraphics* graphics);
	virtual ~Game();

	void Start();

private:
	void InitializeOpenGLES();
	void ClearScreen();
	void KeyCallback(Key key, KeyAction action);
	void Quit();

	void Update(float /*gameDeltaTime*/) {}
	void Render() {}
	void PostRender() {}

	void ProcessMouse();

	IGraphics* m_iGraphics = nullptr;
	IMouse* m_iMouse = nullptr;
	IKeyboard* m_iKeyboard = nullptr;
	SharedInput* m_sharedInput = nullptr;
	SceneManager* m_sceneManager = nullptr;
	Uknitty::Engine* m_engine = nullptr;
	GameplayEvents* m_sharedEvents = nullptr;
	GameManager* m_gameManager = nullptr;

	std::unordered_set<MouseButton> m_mouseButtonStates;
	std::vector<MouseButton> m_mouseButtons = {MouseButton::LEFT, MouseButton::RIGHT, MouseButton::MIDDLE};

	inline static const float m_screenRectangeles[] =
	{
		// Coords    // texCoords
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,

		 1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f
	};

	bool quitting{false};
	float gameDeltaTime;
	int frameCount{0};
	bool m_isWireframeMode = false;
};
