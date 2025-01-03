#include "PhysicsObject.h"

#include "CPhysics.h"
#include "CTransform.h"
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

void PhysicsObject::OverridePosition(glm::vec3 pos)
{
	GameObject::GetLocalTransform()->SetPosition(pos);

	glm::vec3 physicsPos = pos;
	physicsPos.y += m_modelDimensions.y / 2.0f;
	GameObject::GetCPhysics()->SetPosition(physicsPos);
}

void PhysicsObject::SetModelPosToPhysicsPos()
{
	GameObject::GetLocalTransform()->SetPosition(GameObject::GetCPhysics()->GetPosition());
}

} // namespace Uknitty
