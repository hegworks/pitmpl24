#include "SceneGraphNode.h"

#include "Model.h"

void Uknitty::SceneGraphNode::Draw()
{
	if(m_parent)
	{
		m_calculatedTransformMat = (*m_parent->GetCalculatedTransformMat()) * m_localTransformMat;
	}
	else
	{
		m_calculatedTransformMat = m_localTransformMat;
	}

	if(m_model)
	{
		m_model->Draw();
	}

	for(auto child : m_children)
	{
		child->Draw();
	}
}
