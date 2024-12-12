#include "Enemy.h"

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
	glm::vec3 rigidBodyPos = Uknitty::Physics::BtVec3ToGLMVec3(m_physics->GetRigidBody()->getWorldTransform().getOrigin());
	rigidBodyPos.y -= MODEL_DIMENSIONS.y / 2.0;
	m_transform->SetPosition(rigidBodyPos);
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
