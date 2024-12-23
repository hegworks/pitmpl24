#pragma once

#include "RoomChange.h"
#include "RoomChangeType.h"
#include <functional>

namespace Uknitty
{
class CTransform;
class Model;
class ICamera;
class ShaderProgram;
class Physics;
}

class SceneManagerBlackboard;
class btDynamicsWorld;
class btCollisionObject;

class Player
{
#if 0
public:
	Player(Uknitty::Model* model);
	~Player();


	// Inherited via FlowInputRender
	virtual void ProcessMousePosition(double xPos, double yPos) override;
	virtual void KeyDown(Key key) override;
	virtual void KeyUp(Key key) override;
	virtual void ProcessKeyboard(IKeyboard* iKeyboard) override;

	// Inherited via FlowInputRender
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void FixedUpdate() override;
	virtual void Destroy() override;

	// Inherited via FlowInputRender
	virtual void Draw() override;

	Uknitty::Physics* GetPhysics() const { return m_physics; }
	Uknitty::CTransform* GetTransform() const { return m_transform; }
	void SetCollidedWithRoomChangeCallback(std::function<void(RoomChangeType roomChangeType)> callback);
	void RoomChangedSetPosition(RoomChange* roomChange);

private:
	Uknitty::ICamera* m_iCamera = nullptr;
	Uknitty::ShaderProgram* m_shaderProgram = nullptr;
	Uknitty::Physics* m_physics = nullptr;
	btDynamicsWorld* m_btDynamicsWorld = nullptr;
	Uknitty::Model* m_model = nullptr;
	Uknitty::CTransform* m_transform = nullptr;
	SceneManagerBlackboard* m_sceneManagerBlackboard = nullptr;

	const float SPEED_WALK = 1.5f;
	const float SPEED_ROTATION = 1.7f;
	const Key FORWARD_KEY = Key::W;
	const Key BACKWARD_KEY = Key::S;
	const Key LEFT_KEY = Key::A;
	const Key RIGHT_KEY = Key::D;
	const glm::vec3 MODEL_DIMENSIONS = glm::vec3(1.2, 2, 1.2);

	float m_moveSpeed = SPEED_WALK;
	float m_rotationSpeed = SPEED_ROTATION;
	bool m_isForwardKeyDown = false;
	bool m_isBackwardKeyDown = false;
	bool m_isLeftKeyDown = false;
	bool m_isRightKeyDown = false;

	std::function<void(RoomChangeType roomChangeType)> m_collidedWithRoomChangeCallback = nullptr;

	void OnCollision(const btCollisionObject* other);
	void SetGunPos();
#endif
};
