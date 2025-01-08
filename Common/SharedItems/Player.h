#pragma once

#include "DynamicObject.h"
#include "RoomChange.h"
#include "RoomChangeType.h"
#include "UknittySettings.h"
#include <functional>

namespace Uknitty
{
class CTransform;
class CPhysics;
class Model;
class CameraObject;
class ShaderProgram;
class GeneralCamera;
class GameObject;
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
	Uknitty::UserPointerData* m_userPointerData = nullptr;
	Uknitty::GameObject* m_gunPosObject = nullptr;

	const float SPEED_WALK = 10.0f;
	const float SPEED_ROTATION = 20.0f;
	const glm::vec3 MODEL_DIMENSIONS = glm::vec3(1.2, 2, 1.2);
	const float MASS = 70.0f;
	const glm::vec3 GUN_POS = glm::vec3(0, 1.7, 0);

	float m_moveSpeed = SPEED_WALK;
	float m_rotationSpeed = SPEED_ROTATION;

	bool m_lastDrawEnabledStated = true;

	std::function<void(RoomChangeType roomChangeType)> m_collidedWithRoomChangeCallback = nullptr;

	void OnCollision(const btCollisionObject* other);
	void MoveIfInput(float deltaTime);
	void RotateGradually(glm::vec3 dir, float deltaTime);
	void UpdateFeetPos();
	void CheckCameraTypeToDisableDraw();
	void OnShootInput();
	void CastGunRay();
};
