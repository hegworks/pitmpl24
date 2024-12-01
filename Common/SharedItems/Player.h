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

	const float SPEED_WALK = 10.0f;
	const float SPEED_ROTATION = 20.0f;
	const Key FORWARD_KEY = Key::W;
	const Key BACKWARD_KEY = Key::S;
	const Key LEFT_KEY = Key::A;
	const Key RIGHT_KEY = Key::D;

	float m_moveSpeed = SPEED_WALK;
	float m_rotationSpeed = SPEED_WALK;
	bool m_isForwardKeyDown = false;
	bool m_isBackwardKeyDown = false;
	bool m_isLeftKeyDown = false;
	bool m_isRightKeyDown = false;

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
