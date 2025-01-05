#include "DynamicObject.h"

#include "CPhysics.h"
#include "Engine.h"
#include "glm/glm.hpp"

namespace Uknitty
{

void DynamicObject::InitializeWithBoxShape(Model* model, ShaderProgram* shaderProgram, glm::vec3 modelDimensions, float mass, int collisionGroup, int collisionMask)
{
	ModelObject::Initialize(model, shaderProgram);

	m_modelDimensions = modelDimensions;

	CPhysics* cphysics = GameObject::GetCPhysics();
	cphysics->InitialzeWithBoxShape(modelDimensions, mass);

	Uknitty::Engine::GetInstance()->GetDynamicsWorld()->addRigidBody(cphysics->GetRigidBody(), collisionGroup, collisionMask);
}

void DynamicObject::InitializeWithCapsuleShape(Model* model, ShaderProgram* shaderProgram, float radius, float height, float mass, int collisionGroup, int collisionMask)
{
	ModelObject::Initialize(model, shaderProgram);

	m_modelDimensions = glm::vec3(radius, height, radius);

	CPhysics* cphysics = GameObject::GetCPhysics();
	cphysics->InitialzeWithCapsuleShape(radius, height, mass);

	Uknitty::Engine::GetInstance()->GetDynamicsWorld()->addRigidBody(cphysics->GetRigidBody(), collisionGroup, collisionMask);
}

void DynamicObject::OnLateUpdate([[maybe_unused]] float deltaTime)
{
	PhysicsObject::SetModelPosToPhysicsPos();
}

void DynamicObject::MoveInDirection(glm::vec3 direction, float speed)
{
	CPhysics* cphysics = GameObject::GetCPhysics();
	float linearVelocityY = cphysics->GetRigidBody()->getLinearVelocity().getY();
	cphysics->GetRigidBody()->setLinearVelocity(btVector3(direction.x * speed,
														  linearVelocityY + direction.y * speed,
														  direction.z * speed));
}

} // namespace Uknitty
