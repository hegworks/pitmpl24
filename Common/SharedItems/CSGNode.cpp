#include "CSGNode.h"

#include "CRender.h"
#include "CTransform.h"
#include "glm/glm.hpp"

namespace Uknitty
{
#if 0
CSGNode::CSGNode(GameObject* owner)
{
	m_ownerGameObject = owner;
}

void CSGNode::Draw(glm::mat4 parentsMVP)
{
	glm::mat4 transform = parentsMVP * (*m_ownerGameObject->GetLocalTransform()->GetMatrix());
	m_ownerGameObject->GetWorldTransform()->OverrideMatrix(transform);
	if(m_ownerGameObject->HasRender())
	{
		m_ownerGameObject->GetRender()->Draw(transform);
	}
	for(auto& child : m_children)
	{
		child->m_ownerGameObject->GetSGNode()->Draw(transform);
	}
}
#endif
} // namespace Uknitty
