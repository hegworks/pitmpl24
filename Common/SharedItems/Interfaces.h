#pragma once

#include "IInput.h"
#include "IInputKey.h"

namespace Uknitty
{

class GameObject
{
};

#pragma region 1 Base Class

class Input : public GameObject
{

public:
	virtual void ProcessMousePosition(double xPos, double yPos) = 0;
	virtual void ProcessKeyboard(IKeyboard* iKeyboard) = 0;
	virtual void KeyDown(Key key) = 0;
	virtual void KeyUp(Key key) = 0;
	virtual void Destroy() = 0;
};

class Flow : public GameObject
{
public:
	virtual void Awake() = 0;
	virtual void Start() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void LateUpdate(float deltaTime) = 0;
	virtual void FixedUpdate() = 0;
	virtual void Destroy() = 0;
};

class Render : public GameObject
{
public:
	virtual void Draw() = 0;
	virtual void Destroy() = 0;
};

#pragma endregion 1 Base Class

#pragma region 2 Base Classes

class FlowInput : public GameObject
{
public:
	virtual void Awake() = 0;
	virtual void Start() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void LateUpdate(float deltaTime) = 0;
	virtual void FixedUpdate() = 0;
	virtual void Destroy() = 0;

	virtual void ProcessMousePosition(double xPos, double yPos) = 0;
	virtual void ProcessKeyboard(IKeyboard* iKeyboard) = 0;
	virtual void KeyDown(Key key) = 0;
	virtual void KeyUp(Key key) = 0;
};

#pragma endregion 2 Base Classes

#pragma region 3 Base Classes

class FlowInputRender : public GameObject
{
public:
	virtual void Awake() = 0;
	virtual void Start() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void LateUpdate(float deltaTime) = 0;
	virtual void FixedUpdate() = 0;
	virtual void Destroy() = 0;

	virtual void ProcessMousePosition(double xPos, double yPos) = 0;
	virtual void ProcessKeyboard(IKeyboard* iKeyboard) = 0;
	virtual void KeyDown(Key key) = 0;
	virtual void KeyUp(Key key) = 0;

	virtual void Draw() = 0;
};

#pragma endregion 3 Base Classes

} // namespace Uknitty