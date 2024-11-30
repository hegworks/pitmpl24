#pragma once

#include "IInputProcessor.h"

namespace Uknitty
{
class Transform;
class Model;
class ICamera;
}

class Player : public Uknitty::IInputProcessor
{
public:
	Player(Uknitty::Model* model, Uknitty::ICamera* camera);
	~Player();
	Uknitty::Model* m_model = nullptr;
	Uknitty::Transform* m_transform = nullptr;

private:
	Uknitty::ICamera* m_iCamera = nullptr;

	const float SPEED_WALK = 5.0f;

	float m_speed = SPEED_WALK;


	// Inherited via IInputProcessor
	virtual void MouseCallback(double xPos, double yPos);
	virtual void KeyDown(Key key);
	virtual void KeyUp(Key key);
	virtual void ProcessInput(IKeyboard* iKeyboard);
};
