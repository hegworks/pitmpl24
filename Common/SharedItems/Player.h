#pragma once

#include "IInputProcessor.h"

class Transform;
class Model;
class ICamera;

class Player : public IInputProcessor
{
public:
	Player(Model* model, ICamera* camera);
	~Player();
	Model* m_model = nullptr;
	Transform* m_transform = nullptr;

private:
	ICamera* m_iCamera = nullptr;

	const float SPEED_WALK = 5.0f;

	float m_speed = SPEED_WALK;


	// Inherited via IInputProcessor
	virtual void MouseCallback(double xPos, double yPos);
	virtual void KeyDown(Key key);
	virtual void KeyUp(Key key);
	virtual void ProcessInput(IKeyboard* iKeyboard);
};
