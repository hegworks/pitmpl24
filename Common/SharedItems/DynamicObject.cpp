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

void DynamicObject::OnLateUpdate(float deltaTime)
{
	PhysicsObject::SetModelPosToPhysicsPos();
}

} // namespace Uknitty
