#include "PhysicsObject.h"

#include "CPhysics.h"
#include "CTransform.h"
#include "Engine.h"
#include "GameObject.h"
#include "GameObject.h"
#include "ModelObject.h"
#include <iostream>

namespace Uknitty
{

void PhysicsObject::OnAwake()
{
	ModelObject::OnAwake();

	GameObject::AddCPhysics();
}

void PhysicsObject::OnDestroy()
{
	Uknitty::Engine::GetInstance()->GetDynamicsWorld()->removeRigidBody(GameObject::GetCPhysics()->GetRigidBody());

	ModelObject::OnDestroy();
}

void PhysicsObject::OverridePosition(glm::vec3 pos)
{
	GameObject::GetLocalTransform()->SetPosition(pos);

	glm::vec3 physicsPos = pos;
	physicsPos.y += m_modelDimensions.y / 2.0f;
	physicsPos += m_colliderOffset;
	GameObject::GetCPhysics()->SetPosition(physicsPos);
}

void PhysicsObject::SetColliderOffset(glm::vec3 offset)
{
	m_colliderOffset = offset;
	OverridePosition(*GameObject::GetLocalTransform()->GetPosition());
}

void PhysicsObject::SetModelPosToPhysicsPos()
{
	GameObject::GetLocalTransform()->SetPosition(GameObject::GetCPhysics()->GetPosition() + m_colliderOffset);
}

} // namespace Uknitty
