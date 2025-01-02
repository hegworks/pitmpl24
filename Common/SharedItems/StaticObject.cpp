#include "StaticObject.h"

#include "Common.h"
#include "CPhysics.h"
#include "CTransform.h"
#include "Engine.h"
#include "GameObject.h"
#include "ModelObject.h"
#include "PhysicsManager.h"
#include "ShaderProgram.h"
#include "SharedDependencies.h"
#include <iostream>

namespace Uknitty
{

void StaticObject::OnAwake()
{
	ModelObject::OnAwake();

	std::cout << "Awake SolidObject" << std::endl;
	GameObject::AddCPhysics();

}

void StaticObject::InitializeWithBoxShape(Model* model, ShaderProgram* shaderProgram, glm::vec3 modelDimensions, int collisionGroup, int collisionMask)
{
	ModelObject::Initialize(model, shaderProgram);

	m_modelDimensions = modelDimensions;

	CPhysics* cphysics = GameObject::GetCPhysics();
	cphysics->InitialzeWithBoxShape(modelDimensions, 0);

	Uknitty::Engine::GetInstance()->GetDynamicsWorld()->addRigidBody(cphysics->GetRigidBody(), collisionGroup, collisionMask);
}

void StaticObject::OverridePosition(glm::vec3 pos)
{
	glm::vec3 physicsPos = pos;
	physicsPos.y += m_modelDimensions.y / 2.0f;
	GameObject::GetCPhysics()->SetPosition(physicsPos);
	GameObject::GetLocalTransform()->SetPosition(pos);
}

} // namespace Uknitty
