#pragma once

#ifdef WINDOWS_BUILD
#include <GLFW/glfw3.h>

#else
#include <GLES2/gl2.h>

#endif
#include "btBulletDynamicsCommon.h"

#include "IInputKey.h"

struct ImFont;

namespace Uknitty
{
class ICamera;
}

class SharedInput;
class GameObject;
class IGraphics;
class Player;

class Game
{
public:
	Game(SharedInput* input, IGraphics* graphics);
	virtual ~Game();

	void Start();
	void Quit();

	btDiscreteDynamicsWorld* World() const;
	SharedInput* GetInput() const;

protected:
	void ProcessInput();
	virtual void Update(float /*gameDeltaTime*/) {}
	virtual void Render() {}
	virtual void PostRender() {}

	IGraphics* m_iGraphics = nullptr;
	SharedInput* m_sharedInput = nullptr;
	Uknitty::ICamera* m_iCamera = nullptr;
	Player* m_player = nullptr;

	btDefaultCollisionConfiguration* configuration = nullptr;
	btDbvtBroadphase* pairCache = nullptr;
	btCollisionDispatcher* dispatcher = nullptr;
	btSequentialImpulseConstraintSolver* constraintSolver = nullptr;
	btDiscreteDynamicsWorld* world = nullptr;

	bool quitting{false};
	float gameDeltaTime;

private:
	void InitializeOpenGLES();
	void ClearScreen();
	void KeyCallback(Key key, KeyAction action);

	int frameCount{0};
};
