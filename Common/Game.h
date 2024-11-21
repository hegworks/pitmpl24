#pragma once
#include <glm/vec2.hpp>
#include <vector>

#include "Camera.h"

#ifdef WINDOWS_BUILD
#include <GLFW/glfw3.h>

#else
#include <GLES2/gl2.h>

#endif
struct ImFont;
#include "btBulletDynamicsCommon.h"


#ifdef Raspberry_BUILD
#include <GLES3/gl3.h>
#endif

#ifdef WINDOWS_BUILD
#include "glad/glad.h"
#endif



class GameObject;
class IGraphics;
class Input;

class Game
{
public:
	Game(Input* input, IGraphics* graphics);
	virtual ~Game();
	void Start();
	btDiscreteDynamicsWorld* World() const;
	Input* GetInput() const;
	void Quit();
protected:
	void ProcessInput();
	virtual void Update(float /*gameDeltaTime*/) {}
	virtual void Render() {}
	virtual void PostRender() {}

	Input* input;
	bool quitting{false};
	float gameDeltaTime;

	btDefaultCollisionConfiguration* configuration;
	btDbvtBroadphase* pairCache;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* constraintSolver;
	btDiscreteDynamicsWorld* world;
	IGraphics* graphics;
	Camera* camera = nullptr;


private:
	void InitializeOpenGLES();
	void ClearScreen();

	int frameCount{0};
};

