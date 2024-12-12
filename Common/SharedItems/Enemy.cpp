#include "Enemy.h"

#include "EnemyState.h"
#include "glm/glm.hpp"
#include "ICamera.h"
#include "Model.h"
#include "Physics.h"
#include "ShaderProgram.h"
#include "Transform.h"

Enemy::Enemy(Uknitty::Model* model, Uknitty::ICamera* camera, Uknitty::ShaderProgram* shaderProgram, btDynamicsWorld* btDynamicsWorld, std::vector<glm::vec3> patrolPositions)
{
	m_model = model;
	m_iCamera = camera;
	m_shaderProgram = shaderProgram;
	m_btDynamicsWorld = btDynamicsWorld;
	m_patrolPositions = patrolPositions;

	m_transform = new Uknitty::Transform();

	m_physics = new Uknitty::Physics();
	m_physics->InitialzeWithCapsuleShape(glm::vec3(0), MODEL_DIMENSIONS.x / 2.0, MODEL_DIMENSIONS.y / 2.0, 70.0f);
	m_physics->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
	m_physics->GetRigidBody()->setAngularFactor(btVector3(0, 0, 0)); // lock rotation
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
	m_enemyState = EnemyState::PATROL;
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
	switch(m_enemyState)
	{
		case EnemyState::PATROL:
			if(!HasReachedTargetPos())
			{
				MoveTowardTargetPos();
				RotateTowardCurrentDirection();
			}
			else
			{
				ChangeTargetToNextPatrolPos();
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
}

void Enemy::OnCollision(const btCollisionObject* other)
{
}

void Enemy::MoveTowardTargetPos()
{
	glm::vec3 direction = glm::normalize(m_targetPos.pos - GetCurrentFeetPos());
	MoveInDirection(direction);
}

bool Enemy::HasReachedTargetPos()
{
	return glm::distance(GetCurrentFeetPos(), m_targetPos.pos) < DISTANCE_THRESHOLD;
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
	glm::vec3 pos = GetCurrentRigidBodyPos();
	pos.y -= MODEL_DIMENSIONS.y / 2.0;
	return pos;
}
