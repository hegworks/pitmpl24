#pragma once

#include "DynamicObject.h"
#include "GeneralCamera.h"
#include "LightStructs.h"
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
class LightObject;
namespace SkeletalAnimation
{
class Animation;
class Animator;
}
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
	virtual void OnLateUpdate(float deltaTime) override;
	virtual void OnDestroy() override;

	void SetCollidedWithRoomChangeCallback(std::function<void(RoomChangeType roomChangeType)> callback);
	void RoomChangedSetPosition(RoomChange* roomChange);
	void OnEnemyBulletHit();
	void OnUsedHamburger();
	void OnUsedGun();
	int GetMaxHp() const { return HP; }
	int GetHP() const { return m_hp; }
	void OnCameraFolowTypeChanged();

private:
	SceneManagerBlackboard* m_sceneManagerBlackboard = nullptr;
	Uknitty::GeneralCamera* m_generalCamera = nullptr;
	PlayerCInput* m_playerCInput = nullptr;
	Uknitty::UserPointerData* m_userPointerData = nullptr;
	Uknitty::GameObject* m_gunPosObject = nullptr;
	Uknitty::GameObject* m_gunModelObject = nullptr;
	Uknitty::LightObject* m_flashLight = nullptr;
	Uknitty::SkeletalAnimation::Animator* m_animator = nullptr;
	Uknitty::SkeletalAnimation::Animation* m_idleAnim = nullptr;
	Uknitty::SkeletalAnimation::Animation* m_walkAnim = nullptr;
	LightData* m_flashLightData = nullptr;

	const float SPEED_WALK = 6.0f;
	const float SPEED_ROTATION = 15.0f;
	const glm::vec3 MODEL_DIMENSIONS = glm::vec3(1, 2, 1);
	const float MASS = 70.0f;
	const glm::vec3 GUN_POS = glm::vec3(0, 1.7, 0);
	const int HP = 5;
	const glm::vec3 GUN_MODEL_POS_IDLE = glm::vec3(-0.15, 1.5, 0.15);
	const glm::vec3 GUN_MODEL_POS_WALK = glm::vec3(-0.2, 1.25, 0.15);

	float m_moveSpeed = SPEED_WALK;
	float m_rotationSpeed = SPEED_ROTATION;
	int m_hp = HP;

	bool m_lastDrawEnabledState = true;
	Uknitty::GeneralCamera::FollowType m_cameraFollowType = Uknitty::GeneralCamera::FollowType::TOP_DOWN_FOLLOW;

	std::function<void(RoomChangeType roomChangeType)> m_collidedWithRoomChangeCallback = nullptr;

	void OnCollision(const btCollisionObject* other);
	void MoveIfInput(float deltaTime);
	void RotateGradually(glm::vec3 dir, float deltaTime);
	void SetFlashLightDirToCameraIfFirstPerson();
	void UpdateFeetPos();
	void CheckCameraTypeToDisableDraw();
	void OnShootInput();
	void CastGunRay();
};
