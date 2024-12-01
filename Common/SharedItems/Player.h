#pragma once

#include "IInputProcessor.h"
#include "ILifeCycle.h"
#include "IRenderable.h"

namespace Uknitty
{
class Transform;
class Model;
class ICamera;
}

class Player :
	public Uknitty::IInputProcessor,
	public Uknitty::ILifeCycle,
	public Uknitty::IRenderable
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
	virtual void MouseCallback(double xPos, double yPos) override;
	virtual void KeyDown(Key key) override;
	virtual void KeyUp(Key key) override;
	virtual void ProcessInput(IKeyboard* iKeyboard) override;

	// Inherited via ILifeCycle
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void FixedUpdate() override;

	// Inherited via IRenderable
	virtual void Render() override;
};
