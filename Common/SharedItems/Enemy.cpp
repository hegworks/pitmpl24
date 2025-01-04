#include "Enemy.h"

#include "array"
#include "AssetManager.h"
#include "AStar.hpp"
#include "BTDebugDraw.h"
#include "CountdownTimer.h"
#include "CPhysics.h"
#include "CTransform.h"
#include "DynamicObject.h"
#include "EnemyState.h"
#include "Engine.h"
#include "GameObject.h"
#include "GameSettings.h"
#include "GeneralCamera.h"
#include "glm/glm.hpp"
#include "Model.h"
#include "PhysicsCollisionFilters.h"
#include "SceneManagerBlackboard.h"
#include "ShaderProgram.h"
#include "UknittyMath.h"
#include <iostream>

void Enemy::OnAwake()
{
	Uknitty::DynamicObject::OnAwake();

	Uknitty::Model* model = Uknitty::Engine::GetInstance()->GetAssetManager()->AutoGetModel("enemy", "../Common/Assets/Models/Soldier/Soldier.obj");
	Uknitty::ShaderProgram* shaderProgram = Uknitty::Engine::GetInstance()->GetAssetManager()->GetShaderProgram(MAIN_SHADERPROGRAM);
	Uknitty::DynamicObject::InitializeWithCapsuleShape(model, shaderProgram, MODEL_DIMENSIONS.x, MODEL_DIMENSIONS.y, MASS, COLL_GROUP_ENEMY, COLL_MASK_ENEMY);

	m_physics = GameObject::GetCPhysics();
	m_physics->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
	m_physics->GetRigidBody()->setAngularFactor(btVector3(0, 0, 0)); // lock rotation
	m_physics->GetRigidBody()->setRollingFriction(0.0f);
	m_physics->GetRigidBody()->setSpinningFriction(0.0f);
	Uknitty::DynamicObject::SetColliderOffset(glm::vec3(0, -MODEL_DIMENSIONS.y / 2.0, 0));

	auto userPointerData = new Uknitty::CPhysics::UserPointerData();
	userPointerData->physicsType = Uknitty::CPhysics::PhysicsType::ENEMY;
	userPointerData->name = "Enemy";
	m_physics->SetUserPointerData(userPointerData);

	m_physics->SetCollisionCallback([this](const btCollisionObject* other) { OnCollision(other); });

	m_moveSpeed = SPEED_WALK;
	m_rotationSpeed = SPEED_ROTATION;
	m_generalCamera = static_cast<Uknitty::GeneralCamera*>(Uknitty::Engine::GetInstance()->GetMainCamera());
	m_sceneManagerBlackboard = SceneManagerBlackboard::GetInstance();
	m_transform = GameObject::GetLocalTransform();
	m_astarPathGenerationTimer = new Uknitty::CountdownTimer(ASTAR_PATH_GENERATION_DURATION);
}

void Enemy::OnUpdate(float deltaTime)
{
	Uknitty::DynamicObject::OnUpdate(deltaTime);

	DrawAstarPath();

	glm::vec3 physicsPos = Uknitty::DynamicObject::GetCPhysics()->GetPosition();
	glm::vec3 feetPos = GetCurrentFeetPos();
	glm::vec3 targetPos = m_targetPos.pos;
	float distanceToTarget = glm::distance(feetPos, targetPos);
	glm::vec3 playerFeetPos = m_sceneManagerBlackboard->GetPlayerFeetPos();
	float distanceToPlayer = glm::distance(feetPos, playerFeetPos);

	switch(m_enemyState)
	{
		case EnemyState::PATROL:
			if(!HasReachedPatrolTargetPos())
			{
				MoveTowardTargetPos();
				RotateTowardCurrentDirection();
			}
			else
			{
				ChangeTargetToNextPatrolPos();
			}
			if(IsPlayerInSight())
			{
				m_enemyState = EnemyState::ALARM;
			}
			break;
		case EnemyState::ALARM:
			m_astarPathGenerationTimer->Update(deltaTime);

			if(HasReachedPlayerPos())
			{
				SetTransformPosToRigidBodyPos();
				break;
			}

			// we have not reached the player yet
			if(m_astarPathGenerationTimer->IsFinished() ||
			   HasReachedAstarFinalPos() ||
			   !HasAnyAstarPath())
			{
				m_astarPathGenerationTimer->Reset();

				CalculateNewAstarPath();
				ChangeTargetToNextAstarPos();
			}

			MoveTowardTargetPos();
			RotateTowardCurrentDirection();

			if(HasReachedAstarTargetPos())
			{
				ChangeTargetToNextAstarPos();
			}
			break;
	}
}

void Enemy::OnDestroy()
{
	Uknitty::DynamicObject::OnDestroy();

	std::cout << "Destroying Enemy" << std::endl;
}

void Enemy::Initialize(std::vector<glm::vec3> patrolPositions, AStar::Generator* pathFinder)
{
	m_patrolPositions = patrolPositions;
	m_pathFinder = pathFinder;

	Uknitty::DynamicObject::OverridePosition(m_patrolPositions[0]);
	m_sourcePos = {0, m_patrolPositions[0]};
	m_targetPos = {1, m_patrolPositions[1]};
}

void Enemy::DrawAstarPath()
{
#ifdef DEBUG_DRAW_ASTAR_PATH
	btVector3 color = Uknitty::CPhysics::GLMVec3ToBtVec3({1, 1, 0});

	for(int i = m_astarCurrentPathPositions.size() - 1; i >= 0; i--)
	{
		glm::vec2 worldCoord = glm::vec2(m_astarCurrentPathPositions[i].x, m_astarCurrentPathPositions[i].z);
		glLineWidth(2);
		btVector3 lineStart = Uknitty::CPhysics::GLMVec3ToBtVec3({worldCoord.x, 0, worldCoord.y});
		btVector3 lineEnd = Uknitty::CPhysics::GLMVec3ToBtVec3({worldCoord.x, 5, worldCoord.y});
		Uknitty::Engine::GetInstance()->GetDynamicsWorld()->getDebugDrawer()->drawLine(lineStart, lineEnd, color);
		glLineWidth(1);
	}
#endif // DEBUG_DRAW_ASTAR_PATH
}

void Enemy::OnCollision(const btCollisionObject* other)
{
}

void Enemy::MoveTowardTargetPos()
{
	glm::vec3 direction = glm::normalize(m_targetPos.pos - GetCurrentFeetPos());
	MoveInDirection(direction);
}

bool Enemy::HasReachedAstarTargetPos()
{
	return glm::distance(GetCurrentFeetPos(), m_targetPos.pos) < ASTAR_TARGET_DISTANCE_THRESHOLD;
}

bool Enemy::HasReachedPatrolTargetPos()
{
	return glm::distance(GetCurrentFeetPos(), m_targetPos.pos) < PATROL_TARGET_DISTANCE_THRESHOLD;
}

bool Enemy::HasReachedPlayerPos()
{
	return glm::distance(GetCurrentFeetPos(), m_sceneManagerBlackboard->GetPlayerFeetPos()) < PLAYER_DISTANCE_THRESHOLD;
}

void Enemy::ChangeTargetToNextPatrolPos()
{
	m_sourcePos = m_targetPos;
	m_targetPos.index = (m_targetPos.index + 1) % m_patrolPositions.size();
	m_targetPos.pos = m_patrolPositions[m_targetPos.index];
}

/// <summary>
/// Moves the enemy in the given direction which MUST be passed to it normalized.
/// This function does NOT normalize the direction for you. (to prevent double normalization).
/// </summary>
/// <param name="direction"></param>
void Enemy::MoveInDirection(glm::vec3 direction)
{
	float linearVelocityY = m_physics->GetRigidBody()->getLinearVelocity().getY();
	m_physics->GetRigidBody()->setLinearVelocity(btVector3(direction.x * m_moveSpeed,
														   linearVelocityY + direction.y * m_moveSpeed,
														   direction.z * m_moveSpeed));
	SetTransformPosToRigidBodyPos();
}

void Enemy::RotateTowardCurrentDirection()
{
	glm::vec3 direction = glm::normalize(Uknitty::CPhysics::BtVec3ToGLMVec3(m_physics->GetRigidBody()->getLinearVelocity()));
	m_lastDirection = direction;
	float targetAngle = atan2(direction.x, direction.z);
	glm::vec3 rotation = glm::vec3(0, glm::degrees(targetAngle), 0);
	m_transform->SetRotation(rotation);
}

void Enemy::SetTransformPosToRigidBodyPos()
{
	m_transform->SetPosition(GetCurrentFeetPos());
}

glm::vec3 Enemy::GetCurrentFeetPos()
{
	return Uknitty::DynamicObject::GetCPhysics()->GetPosition() - glm::vec3(0, MODEL_DIMENSIONS.y / 2.0, 0);
}

glm::vec3 Enemy::GetHeadPos()
{
	return Uknitty::DynamicObject::GetCPhysics()->GetPosition() + glm::vec3(0, MODEL_DIMENSIONS.y / 2.0, 0);
}

void Enemy::CalculateNewAstarPath()
{
	// enmey
	glm::vec3 sourceWorldPos = GetCurrentFeetPos();
	glm::vec2 rawSourceWorldCoord = {sourceWorldPos.x, sourceWorldPos.z};
	glm::ivec2 sourceAstarCoord = FindUncollisionedAstarCoord(rawSourceWorldCoord);

	// player
	glm::vec3 rawTargetWorldPos = m_sceneManagerBlackboard->GetPlayerFeetPos();
	glm::vec2 targetWorldCoord = {rawTargetWorldPos.x, rawTargetWorldPos.z};
	glm::ivec2 targetAstarCoord = FindUncollisionedAstarCoord(targetWorldCoord);

	const auto path = m_pathFinder->findPath({sourceAstarCoord.x, sourceAstarCoord.y}, {targetAstarCoord.x, targetAstarCoord.y});

	ClearAstarPath();

	for(int i = path.size() - ASTAR_PATH_SKIP_BEGINNING_COUNT; i >= 0; i--)
	{
		const glm::ivec2 astarCoord = {path[i].x, path[i].y};
		glm::vec2 worldCoord = AstarCoordToWorldCoord(astarCoord);
		m_astarCurrentPathPositions.push_back(glm::vec3(worldCoord.x, 0, worldCoord.y));
	}
}

void Enemy::ClearAstarPath()
{
	m_astarCurrentPathPositions.clear();
	m_astarCurrentPathIndex = -1;
}

void Enemy::ChangeTargetToNextAstarPos()
{
	m_astarCurrentPathIndex = (m_astarCurrentPathIndex + 1) % m_astarCurrentPathPositions.size();
	m_targetPos.pos = m_astarCurrentPathPositions[m_astarCurrentPathIndex];
}

bool Enemy::HasReachedAstarFinalPos()
{
	return m_astarCurrentPathIndex == m_astarCurrentPathPositions.size() - 1;
}

bool Enemy::HasAnyAstarPath()
{
	return m_astarCurrentPathPositions.size() > 0;
}

bool Enemy::IsCoordCollisionInAstar(glm::ivec2 coord)
{
	return m_pathFinder->detectCollision({coord.x,coord.y});
}

glm::vec2 Enemy::WorldCoordToAstarCoord(glm::vec2 worldCoord)
{
	float x = Uknitty::Math::range_to_range(16, -16, 0, 31, worldCoord.x);
	float y = Uknitty::Math::range_to_range(12, -12, 0, 23, worldCoord.y);
	return {x, y};
}

glm::vec2 Enemy::AstarCoordToWorldCoord(glm::ivec2 astarCoord)
{
	float x = floor(Uknitty::Math::range_to_range(0, 31, 16, -16, astarCoord.x)) + 0.5f;
	float y = floor(Uknitty::Math::range_to_range(0, 23, 12, -12, astarCoord.y)) + 0.5f;
	return {x, y};
}

glm::ivec2 Enemy::FindUncollisionedAstarCoord(glm::vec2 rawWorldCoord)
{
	glm::vec2 wFxFy = glm::vec2(std::floor(rawWorldCoord.x), std::floor(rawWorldCoord.y));
	{
		glm::vec2 a = WorldCoordToAstarCoord(wFxFy);
		glm::ivec2 aFxFy = glm::ivec2(std::floor(a.x), std::floor(a.y));
		if(!IsCoordCollisionInAstar({aFxFy.x, aFxFy.y})) aFxFy;
		glm::ivec2 aFxCy = glm::ivec2(std::floor(a.x), std::ceil(a.y));
		if(!IsCoordCollisionInAstar({aFxCy.x, aFxCy.y})) return aFxCy;
		glm::ivec2 aCxFy = glm::ivec2(std::ceil(a.x), std::floor(a.y));
		if(!IsCoordCollisionInAstar({aCxFy.x, aCxFy.y})) return aCxFy;
		glm::ivec2 aCxCy = glm::ivec2(std::ceil(a.x), std::ceil(a.y));
		if(!IsCoordCollisionInAstar({aCxCy.x, aCxCy.y})) return aCxCy;
	}

	glm::vec2 wFxCy = glm::vec2(std::floor(rawWorldCoord.x), std::ceil(rawWorldCoord.y));
	{
		glm::vec2 a = WorldCoordToAstarCoord(wFxCy);
		glm::ivec2 aFxFy = glm::ivec2(std::floor(a.x), std::floor(a.y));
		if(!IsCoordCollisionInAstar({aFxFy.x, aFxFy.y})) return aFxFy;
		glm::ivec2 aFxCy = glm::ivec2(std::floor(a.x), std::ceil(a.y));
		if(!IsCoordCollisionInAstar({aFxCy.x, aFxCy.y})) return  aFxCy;
		glm::ivec2 aCxFy = glm::ivec2(std::ceil(a.x), std::floor(a.y));
		if(!IsCoordCollisionInAstar({aCxFy.x, aCxFy.y})) return aCxFy;
		glm::ivec2 aCxCy = glm::ivec2(std::ceil(a.x), std::ceil(a.y));
		if(!IsCoordCollisionInAstar({aCxCy.x, aCxCy.y})) return aCxCy;
	}

	glm::vec2 wCxFy = glm::vec2(std::ceil(rawWorldCoord.x), std::floor(rawWorldCoord.y));
	{
		glm::vec2 a = WorldCoordToAstarCoord(wCxFy);
		glm::ivec2 aFxFy = glm::ivec2(std::floor(a.x), std::floor(a.y));
		if(!IsCoordCollisionInAstar({aFxFy.x, aFxFy.y})) return aFxFy;
		glm::ivec2 aFxCy = glm::ivec2(std::floor(a.x), std::ceil(a.y));
		if(!IsCoordCollisionInAstar({aFxCy.x, aFxCy.y})) return aFxCy;
		glm::ivec2 aCxFy = glm::ivec2(std::ceil(a.x), std::floor(a.y));
		if(!IsCoordCollisionInAstar({aCxFy.x, aCxFy.y})) return aCxFy;
		glm::ivec2 aCxCy = glm::ivec2(std::ceil(a.x), std::ceil(a.y));
		if(!IsCoordCollisionInAstar({aCxCy.x, aCxCy.y})) return aCxCy;
	}

	glm::vec2 wCxCy = glm::vec2(std::ceil(rawWorldCoord.x), std::ceil(rawWorldCoord.y));
	{
		glm::vec2 a = WorldCoordToAstarCoord(wCxCy);
		glm::ivec2 aFxFy = glm::ivec2(std::floor(a.x), std::floor(a.y));
		if(!IsCoordCollisionInAstar({aFxFy.x, aFxFy.y})) return aFxFy;
		glm::ivec2 aFxCy = glm::ivec2(std::floor(a.x), std::ceil(a.y));
		if(!IsCoordCollisionInAstar({aFxCy.x, aFxCy.y})) return aFxCy;
		glm::ivec2 aCxFy = glm::ivec2(std::ceil(a.x), std::floor(a.y));
		if(!IsCoordCollisionInAstar({aCxFy.x, aCxFy.y})) return aCxFy;
		glm::ivec2 aCxCy = glm::ivec2(std::ceil(a.x), std::ceil(a.y));
		if(!IsCoordCollisionInAstar({aCxCy.x, aCxCy.y})) return aCxCy;
	}

	throw new std::runtime_error("No uncollisioned astar coord found");
}

bool Enemy::IsPlayerInSight()
{
	const glm::vec3 headPos = GetHeadPos();
	glm::vec3 dir = m_transform->GetForward();
	const btVector3 from = Uknitty::CPhysics::GLMVec3ToBtVec3(headPos);
	const btVector3 centeredTo = Uknitty::CPhysics::GLMVec3ToBtVec3(headPos + (dir * SIGHT_RAY_LENGTH));

	int sideCounter = 0;
	for(int i = 0; i < SIGHT_RAY_COUNT; i++)
	{
		btVector3 to = centeredTo;
		float angle = (i % 2 == 0 ? 1 : -1) * ((i + 1) / 2) * SIGHT_RAY_DIFFERENCE_DEGREE;
		glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
		glm::vec3 rotatedDir = glm::vec3(rotationMat * glm::vec4(dir, 1.0f));
		to = from + Uknitty::CPhysics::GLMVec3ToBtVec3(rotatedDir) * SIGHT_RAY_LENGTH;

#ifdef DEBUG_DRAW_PHYSICS
		m_btDynamicsWorld->getDebugDrawer()->drawLine(from, to, Uknitty::Physics::GetBtColor(Uknitty::Physics::Color::PINK));
#endif // DEBUG_DRAW_PHYSICS 

		btCollisionWorld::ClosestRayResultCallback  closestResults = btCollisionWorld::ClosestRayResultCallback(from, to);
		closestResults.m_collisionFilterGroup = COLL_GROUP_ENEMY;
		closestResults.m_collisionFilterMask = COLL_MASK_ENEMY;
		//closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;

		Uknitty::Engine::GetInstance()->GetDynamicsWorld()->rayTest(from, to, closestResults);

		if(closestResults.hasHit())
		{
			btVector3 p = from.lerp(to, closestResults.m_closestHitFraction);

#ifdef DEBUG_DRAW_PHYSICS 
			m_btDynamicsWorld->getDebugDrawer()->drawSphere(p, 0.1, Uknitty::Physics::GetBtColor(Uknitty::Physics::Color::CYAN));
			m_btDynamicsWorld->getDebugDrawer()->drawLine(p, p + closestResults.m_hitNormalWorld, Uknitty::Physics::GetBtColor(Uknitty::Physics::Color::CYAN));
#endif // DEBUG_DRAW_PHYSICS 

			if(closestResults.m_collisionObject->getUserPointer())
			{
				auto userPointerData = static_cast<Uknitty::CPhysics::UserPointerData*>(closestResults.m_collisionObject->getUserPointer());
				if(userPointerData->physicsType == Uknitty::CPhysics::PhysicsType::PLAYER)
				{
					return true;
				}
			}
		}
	}
	return false;
}
