#pragma once

#ifdef WINDOWS_BUILD
#include <GLFW/glfw3.h>

#else
#include <GLES2/gl2.h>

#endif
#include "btBulletDynamicsCommon.h"

#include "IInputKey.h"
#include <vector>

struct ImFont;

namespace Uknitty
{
class ICamera;
class IInputProcessor;
class ILifeCycle;
class IRenderable;
}

class SharedInput;
class IGraphics;
class IMouse;
class IKeyboard;

class Player;

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

	IGraphics* m_iGraphics = nullptr;
	IMouse* m_iMouse = nullptr;
	IKeyboard* m_iKeyboard = nullptr;
	SharedInput* m_sharedInput = nullptr;
	Uknitty::ICamera* m_iCamera = nullptr;
	Player* m_player = nullptr;
	std::vector<Uknitty::IInputProcessor*> m_iInputProcessors;
	std::vector<Uknitty::IRenderable*> m_iRenderables;
	std::vector<Uknitty::ILifeCycle*> m_iLifeCycles;

	btDefaultCollisionConfiguration* configuration = nullptr;
	btDbvtBroadphase* pairCache = nullptr;
	btCollisionDispatcher* dispatcher = nullptr;
	btSequentialImpulseConstraintSolver* constraintSolver = nullptr;
	btDiscreteDynamicsWorld* world = nullptr;

	bool quitting{false};
	float gameDeltaTime;
	int frameCount{0};
};
