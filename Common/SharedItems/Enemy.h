#pragma once

#include "AStar.hpp"
#include "EnemyState.h"
#include "glm/glm.hpp"
#include "Interfaces.h"
#include <vector>

namespace Uknitty
{
class Transform;
class Model;
class ICamera;
class ShaderProgram;
class Physics;
class BTDebugDraw;
class CountdownTimer;
}

class btDynamicsWorld;
class btCollisionObject;
class SceneManagerBlackboard;

class Enemy : public Uknitty::FlowRender
{
public:
	Enemy(Uknitty::Model* model, std::vector<glm::vec3> patrolPositions, AStar::Generator* pathFinder);
	~Enemy();

	// Inherited via FlowRender
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void FixedUpdate() override;
	virtual void Destroy() override;
	virtual void Draw() override;

	Uknitty::Physics* GetPhysics() const { return m_physics; }
	Uknitty::Transform* GetTransform() const { return m_transform; }

private:
	struct PosData
	{
		int index; // index of m_patrolPositions
		glm::vec3 pos;
	};

	Uknitty::ICamera* m_iCamera = nullptr;
	Uknitty::ShaderProgram* m_shaderProgram = nullptr;
	Uknitty::Physics* m_physics = nullptr;
	btDynamicsWorld* m_btDynamicsWorld = nullptr;
	Uknitty::Model* m_model = nullptr;
	Uknitty::Transform* m_transform = nullptr;
	AStar::Generator* m_pathFinder = nullptr;
	SceneManagerBlackboard* m_sceneManagerBlackboard = nullptr;
	Uknitty::CountdownTimer* m_astarPathGenerationTimer = nullptr;

	const float SPEED_WALK = 0.7f;
	const float SPEED_ROTATION = 1.7f;
	const glm::vec3 MODEL_DIMENSIONS = glm::vec3(0.8, 2, 0.8);
	const float ASTAR_TARGET_DISTANCE_THRESHOLD = 1.0f; // deadzone for reaching astar target position
	const float PATROL_TARGET_DISTANCE_THRESHOLD = 0.2f; // deadzone for reaching patrol target position
	const float PLAYER_DISTANCE_THRESHOLD = 4.0f; // deadzone for reaching player position
	const float ASTAR_PATH_GENERATION_DURATION = 0.5f; // generate new path every x seconds
	const int ASTAR_PATH_SKIP_BEGINNING_COUNT = 3; // skip first x nodes of the path
	const float SIGHT_RAY_LENGTH = 30.0f;
	const int SIGHT_RAY_COUNT = 3; // for symmetrical results, set this to an odd value
	const float SIGHT_RAY_DIFFERENCE_DEGREE = 5.0f;

	float m_moveSpeed = SPEED_WALK;
	float m_rotationSpeed = SPEED_ROTATION;
	std::vector<glm::vec3> m_patrolPositions;
	PosData m_sourcePos;
	PosData m_targetPos;
	std::vector<glm::vec3> m_astarCurrentPathPositions;
	int m_astarCurrentPathIndex = 0;
	bool m_hadReachedPlayerPos = false;
	glm::vec3 m_lastDirection;

	EnemyState m_enemyState = EnemyState::PATROL;

	void OnCollision(const btCollisionObject* other);
	void MoveTowardTargetPos();
	bool HasReachedAstarTargetPos();
	bool HasReachedPatrolTargetPos();
	bool HasReachedPlayerPos();
	void ChangeTargetToNextPatrolPos();
	void MoveInDirection(glm::vec3 direction);
	void RotateTowardCurrentDirection();
	void SetTransformPosToRigidBodyPos();
	glm::vec3 GetCurrentRigidBodyPos();
	glm::vec3 GetCurrentFeetPos();
	glm::vec3 GetHeadPos();
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
};
