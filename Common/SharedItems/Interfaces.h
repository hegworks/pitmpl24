#pragma once

namespace Uknitty
{

class GameObject
{
};

#pragma region 1 Base Class

class Input : public GameObject
{

#include "IInput.h"
#include "IInputKey.h"

public:
	virtual void MouseCallback(double xPos, double yPos) = 0;
	virtual void KeyDown(Key key) = 0;
	virtual void KeyUp(Key key) = 0;
	virtual void ProcessInput(IKeyboard* iKeyboard) = 0;
};

class Flow : public GameObject
{
public:
	virtual void Awake() = 0;
	virtual void Start() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void LateUpdate(float deltaTime) = 0;
	virtual void FixedUpdate() = 0;
};

class Render : public GameObject
{
public:
	virtual void Draw() = 0;
};

#pragma endregion 1 Base Class

#pragma region 2 Base Classes

class FlowInput : public Input
{
public:
	virtual void Awake() = 0;
	virtual void Start() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void LateUpdate(float deltaTime) = 0;
	virtual void FixedUpdate() = 0;
};

#pragma endregion 2 Base Classes

#pragma region 3 Base Classes

class FlowInputRender : public FlowInput
{
public:
	virtual void Draw() = 0;
};

#pragma endregion 3 Base Classes

} // namespace Uknitty