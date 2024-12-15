#include "Enemy.h"

#include "AStar.hpp"
#include "BTDebugDraw.h"
#include "Common.h"
#include "CountdownTimer.h"
#include "EnemyState.h"
#include "glm/glm.hpp"
#include "ICamera.h"
#include "Model.h"
#include "Physics.h"
#include "SceneManagerBlackboard.h"
#include "ShaderProgram.h"
#include "Transform.h"
#include "UknittyMath.h"
#include <iostream>

Enemy::Enemy(Uknitty::Model* model, Uknitty::ICamera* camera, Uknitty::ShaderProgram* shaderProgram, btDynamicsWorld* btDynamicsWorld, std::vector<glm::vec3> patrolPositions, AStar::Generator* pathFinder, SceneManagerBlackboard* sceneManagerBlackboard)
{
	m_model = model;
	m_iCamera = camera;
	m_shaderProgram = shaderProgram;
	m_btDynamicsWorld = btDynamicsWorld;
	m_patrolPositions = patrolPositions;
	m_pathFinder = pathFinder;
	m_sceneManagerBlackboard = sceneManagerBlackboard;

	m_transform = new Uknitty::Transform();
	m_astarPathGenerationTimer = new Uknitty::CountdownTimer(ASTAR_PATH_GENERATION_DURATION);

	m_physics = new Uknitty::Physics();
	m_physics->InitialzeWithCapsuleShape(glm::vec3(0), MODEL_DIMENSIONS.x / 2.0, MODEL_DIMENSIONS.y / 2.0, 70.0f);
	m_physics->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
	m_physics->GetRigidBody()->setAngularFactor(btVector3(0, 0, 0)); // lock rotation
	m_physics->GetRigidBody()->setRollingFriction(0.0f);
	m_physics->GetRigidBody()->setSpinningFriction(0.0f);
	auto userPointerData = new Uknitty::Physics::UserPointerData();
	userPointerData->physicsType = Uknitty::Physics::PhysicsType::ENEMY;
	userPointerData->name = "Enemy";
	m_physics->SetUserPointerData(userPointerData);
	m_physics->SetCollisionCallback([this](const btCollisionObject* other) { OnCollision(other); });
	m_physics->SetPosition(m_patrolPositions[0]);
	SetTransformPosToRigidBodyPos();
	m_sourcePos = {0, m_patrolPositions[0]};
	m_targetPos = {1, m_patrolPositions[1]};
	m_moveSpeed = SPEED_WALK;
	m_rotationSpeed = SPEED_ROTATION;

	//CalculateNewAstarPath();
	//ChangeTargetToNextAstarPos();
	//m_enemyState = EnemyState::ALARM;

#ifdef WINDOWS_BUILD
	m_btDynamicsWorld->DebugAddRigidBody(m_physics->GetRigidBody(), "Enemy");
#elif Raspberry_BUILD
	m_btDynamicsWorld->addRigidBody(m_physics->GetRigidBody());
#endif
}

Enemy::~Enemy()
{
#ifdef WINDOWS_BUILD
	m_btDynamicsWorld->DebugRemoveRigidBody(m_physics->GetRigidBody(), "Enemy");
#elif Raspberry_BUILD
	m_btDynamicsWorld->removeRigidBody(m_physics->GetRigidBody());
#endif
	delete m_physics;
	delete m_transform;
}

void Enemy::Awake()
{
}

void Enemy::Start()
{
}

void Enemy::Update(float deltaTime)
{
	m_astarPathGenerationTimer->Update(deltaTime);

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
			if(HasReachedPlayerPos())
			{
				SetTransformPosToRigidBodyPos();
				break;
			}

			// we have not reached the player yet
			if(m_astarPathGenerationTimer->IsFinished() || HasReachedAstarFinalPos() || !HasAnyAstarPath())
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

void Enemy::LateUpdate(float deltaTime)
{
}

void Enemy::FixedUpdate()
{
}

void Enemy::Destroy()
{
	std::cout << "Destroying Enemy" << std::endl;
	delete this;
}

void Enemy::Draw()
{
	m_shaderProgram->Use();
	m_shaderProgram->SetMat4("uView", m_iCamera->GetView());
	m_shaderProgram->SetMat4("uProjection", m_iCamera->GetProjection());
	m_shaderProgram->SetMat4("uModel", *m_transform->GetModelMatrix());
	glDisable(GL_BLEND);
	m_model->Draw(*m_shaderProgram);
	m_shaderProgram->UnUse();

#ifdef DEBUG_DRAW_ASTAR_PATH
	btVector3 color = Uknitty::Physics::GLMVec3ToBtVec3({1, 1, 0});

	for(int i = m_astarCurrentPathPositions.size() - 1; i >= 0; i--)
	{
		glm::vec2 worldCoord = glm::vec2(m_astarCurrentPathPositions[i].x, m_astarCurrentPathPositions[i].z);
		glLineWidth(2);
		btVector3 lineStart = Uknitty::Physics::GLMVec3ToBtVec3({worldCoord.x, 0, worldCoord.y});
		btVector3 lineEnd = Uknitty::Physics::GLMVec3ToBtVec3({worldCoord.x, 5, worldCoord.y});
		m_btDynamicsWorld->getDebugDrawer()->drawLine(lineStart, lineEnd, color);
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
	glm::vec3 direction = glm::normalize(Uknitty::Physics::BtVec3ToGLMVec3(m_physics->GetRigidBody()->getLinearVelocity()));
	m_lastDirection = direction;
	float targetAngle = atan2(direction.x, direction.z);
	glm::vec3 rotation = glm::vec3(0, glm::degrees(targetAngle), 0);
	m_transform->SetRotation(rotation);
}

void Enemy::SetTransformPosToRigidBodyPos()
{
	m_transform->SetPosition(GetCurrentFeetPos());
}

glm::vec3 Enemy::GetCurrentRigidBodyPos()
{
	return Uknitty::Physics::BtVec3ToGLMVec3(m_physics->GetRigidBody()->getWorldTransform().getOrigin());
}

glm::vec3 Enemy::GetCurrentFeetPos()
{
	return GetCurrentRigidBodyPos() - glm::vec3(0, MODEL_DIMENSIONS.y / 2.0, 0);
}

glm::vec3 Enemy::GetHeadPos()
{
	return GetCurrentRigidBodyPos() + glm::vec3(0, MODEL_DIMENSIONS.y / 2.0, 0);
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
		const glm::vec2 worldCoord = AstarCoordToWorldCoord(astarCoord);
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
	int x = Uknitty::Math::range_to_range(0, 31, 16, -16, astarCoord.x);
	int y = Uknitty::Math::range_to_range(0, 23, 12, -12, astarCoord.y);
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
	glm::vec3 headPos = GetHeadPos();
	glm::vec3 dir = m_transform->GetForward();
	btVector3 from = Uknitty::Physics::GLMVec3ToBtVec3(headPos);
	btVector3 to = Uknitty::Physics::GLMVec3ToBtVec3(headPos + (dir * SIGHT_RAY_LENGTH));
	to.setY(from.getY());
	//std::cout << "from: " << from.x() << " " << from.y() << " " << from.z() << std::endl;
	//std::cout << "to: " << to.x() << " " << to.y() << " " << to.z() << std::endl;

#ifdef DEBUG_DRAW_PHYSICS 
	m_btDynamicsWorld->getDebugDrawer()->drawLine(from, to, Uknitty::Physics::GetBtColor(Uknitty::Physics::Color::PINK));
#endif // DEBUG_DRAW_PHYSICS 

	btCollisionWorld::ClosestRayResultCallback closestResults(from, to);
	//closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;

	m_btDynamicsWorld->rayTest(from, to, closestResults);

	if(closestResults.hasHit())
	{
		btVector3 p = from.lerp(to, closestResults.m_closestHitFraction);

#ifdef DEBUG_DRAW_PHYSICS 
		m_btDynamicsWorld->getDebugDrawer()->drawSphere(p, 0.1, Uknitty::Physics::GetBtColor(Uknitty::Physics::Color::CYAN));
		m_btDynamicsWorld->getDebugDrawer()->drawLine(p, p + closestResults.m_hitNormalWorld, Uknitty::Physics::GetBtColor(Uknitty::Physics::Color::CYAN));
#endif // DEBUG_DRAW_PHYSICS 

		if(closestResults.m_collisionObject->getUserPointer())
		{
			auto userPointerData = static_cast<Uknitty::Physics::UserPointerData*>(closestResults.m_collisionObject->getUserPointer());
			if(userPointerData->physicsType == Uknitty::Physics::PhysicsType::PLAYER)
			{
				return true;
			}
		}
	}
	return false;
}
