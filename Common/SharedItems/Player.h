#pragma once

#include "DynamicObject.h"
#include "RoomChange.h"
#include "RoomChangeType.h"
#include <functional>

namespace Uknitty
{
class CTransform;
class Model;
class CameraObject;
class ShaderProgram;
class Physics;
class GeneralCamera;
}

class SceneManagerBlackboard;
class btDynamicsWorld;
class btCollisionObject;
class PlayerCInput;

class Player : public Uknitty::DynamicObject
{
public:
	virtual void OnAwake() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnDestroy() override;

	void SetCollidedWithRoomChangeCallback(std::function<void(RoomChangeType roomChangeType)> callback);
	void RoomChangedSetPosition(RoomChange* roomChange);

private:
	SceneManagerBlackboard* m_sceneManagerBlackboard = nullptr;
	Uknitty::GeneralCamera* m_generalCamera = nullptr;
	PlayerCInput* m_playerCInput = nullptr;

	const float SPEED_WALK = 1.5f;
	const float SPEED_ROTATION = 1.7f;
	const glm::vec3 MODEL_DIMENSIONS = glm::vec3(1.2, 2, 1.2);
	const float MASS = 70.0f;

	float m_moveSpeed = SPEED_WALK;
	float m_rotationSpeed = SPEED_ROTATION;

	bool m_lastDrawEnabledStated = true;

	std::function<void(RoomChangeType roomChangeType)> m_collidedWithRoomChangeCallback = nullptr;

	void OnCollision(const btCollisionObject* other);
	void MoveIfInput(float deltaTime);
	void RotateGradually(glm::vec3 dir, float deltaTime);
	void UpdateFeetPos();
	void CheckCameraTypeToDisableDraw();
};
