#pragma once

#include "AStar.hpp"
#include "DynamicObject.h"
#include "EnemyState.h"
#include "glm/glm.hpp"
#include "UknittySettings.h"
#include <memory>
#include <vector>

namespace Uknitty
{
class CTransform;
class Model;
class CameraObject;
class ShaderProgram;
class Physics;
class BTDebugDraw;
class CountdownTimer;
class DynamicObject;
class GeneralCamera;
class GameObject;
class CPhysics;
namespace SkeletalAnimation
{
class Animation;
class Animator;
}
}
class btDynamicsWorld;
class btCollisionObject;
class SceneManagerBlackboard;
class GameplayEvents;

class Enemy : public Uknitty::DynamicObject
{
public:
	virtual ~Enemy() override = default;
	virtual void OnAwake() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnDestroy() override;

	void Initialize(std::vector<glm::vec3> patrolPositions, AStar::Generator* pathFinder);
	void DrawAstarPath();
	void OnPlayerBulletHit();
	/// this function should only be called from other classes.
	void EnterAlarmState();

private:
	struct PosData
	{
		int index; // index of m_patrolPositions
		glm::vec3 pos;
	};

	Uknitty::GeneralCamera* m_generalCamera = nullptr;
	AStar::Generator* m_pathFinder = nullptr;
	SceneManagerBlackboard* m_sceneManagerBlackboard = nullptr;
	GameplayEvents* m_gameplayEvents = nullptr;
	Uknitty::CountdownTimer* m_astarPathGenerationTimer = nullptr;
	Uknitty::CountdownTimer* m_shootTimer = nullptr;
	Uknitty::CTransform* m_transform = nullptr;
	Uknitty::CPhysics* m_physics = nullptr;
	Uknitty::UserPointerData* m_userPointerData = nullptr;
	Uknitty::GameObject* m_gunPosObject = nullptr;
	Uknitty::SkeletalAnimation::Animator* m_animator = nullptr;
	Uknitty::SkeletalAnimation::Animation* m_idleAnim = nullptr;
	Uknitty::SkeletalAnimation::Animation* m_walkAnim = nullptr;
	Uknitty::SkeletalAnimation::Animation* m_deathAnim = nullptr;

	const float SPEED_WALK = 3.5f;
	const float SPEED_ROTATION = 20.0f;
	const glm::vec3 SCALE = glm::vec3(1.2f);
	const glm::vec3 MODEL_DIMENSIONS = glm::vec3(1.0, 2.3, 1.0);
	const float ASTAR_TARGET_DISTANCE_THRESHOLD = 0.2f; // deadzone for reaching astar target position
	const float PATROL_TARGET_DISTANCE_THRESHOLD = 0.2f; // deadzone for reaching patrol target position
	const float PLAYER_DISTANCE_THRESHOLD = 6.0f; // deadzone for reaching player position
	const float ASTAR_PATH_GENERATION_DURATION = 0.5f; // generate new path every x seconds
	const int ASTAR_PATH_SKIP_BEGINNING_COUNT = 3; // skip first x nodes of the path
	const float SIGHT_RAY_LENGTH = 30.0f;
	const float GUN_SHOOT_RAY_LENGTH = 30.0f;
	const int SIGHT_RAY_COUNT = 7; // for symmetrical results, set this to an odd value
	const float SIGHT_RAY_DIFFERENCE_DEGREE = 4.0f;
	const float MASS = 70.0f;
	const int HP = 3;
	const glm::vec3 GUN_POS = glm::vec3(0, 1.0, 0.5);
	const float SHOOT_FREQUENCY_TIME = 2.0; // shoot the gun every x seconds

	float m_moveSpeed = SPEED_WALK;
	float m_rotationSpeed = SPEED_ROTATION;
	std::vector<glm::vec3> m_patrolPositions;
	PosData m_sourcePos;
	PosData m_targetPos;
	std::vector<glm::vec3> m_astarCurrentPathPositions;
	int m_astarCurrentPathIndex = 0;
	bool m_hadReachedPlayerPos = false;
	glm::vec3 m_lastDirection;
	int m_hp = HP;

	EnemyState m_enemyState = EnemyState::PATROL;

	void OnCollision(const btCollisionObject* other);
	void MoveTowardTargetPos();
	bool HasReachedAstarTargetPos();
	bool HasReachedPatrolTargetPos();
	bool HasReachedPlayerPos();
	void ChangeTargetToNextPatrolPos();
	void RotateTowardCurrentDirection();
	void SetTransformPosToRigidBodyPos();
	glm::vec3 GetCurrentFeetPos();
	glm::vec3 GetEyePos();
	void CalculateNewAstarPath();
	void ClearAstarPath();
	void ChangeTargetToNextAstarPos();
	bool HasReachedAstarFinalPos();
	bool HasAnyAstarPath();
	bool IsCoordCollisionInAstar(glm::ivec2 coord); //TODO move to AStar class
	glm::vec2 WorldCoordToAstarCoord(glm::vec2 worldCoord); //TODO move to AStar class
	glm::vec2 AstarCoordToWorldCoord(glm::ivec2 astarCoord); //TODO move to AStar class
	glm::ivec2 FindUncollisionedAstarCoord(glm::vec2 rawWorldCoord); //TODO move to AStar class
	bool IsPlayerInSight();
	void ShootGun();
	void RotateTowardPlayer();
	void StopMoving();
};
