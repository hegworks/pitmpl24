#include "GameObject.h"

#include "CInput.h"
#include "CPhysics.h"
#include "CRender.h"
#include "CTransform.h"
#include "Engine.h"
#include "glm/glm.hpp"
#include <unordered_set>

namespace Uknitty
{

GameObject::GameObject()
{
	m_localTransform = new CTransform();
	m_worldTransform = new CTransform();
}

GameObject::~GameObject()
{
	delete m_localTransform;
	delete m_worldTransform;
	if(HasCInput())
	{
		delete m_input;
	}
	if(HasCRender())
	{
		delete m_render;
	}
	if(HasCPhysics())
	{
		Uknitty::Engine::GetInstance()->GetDynamicsWorld()->removeRigidBody(GameObject::GetCPhysics()->GetRigidBody());
		delete m_physics;
	}
}

void GameObject::EnableDrawChildren()
{
	for(auto& gameObject : m_children)
	{
		gameObject->EnableDrawSelf();
		gameObject->EnableDrawChildren();
	}
}

void GameObject::DisableDrawChildren()
{
	for(auto& gameObject : m_children)
	{
		gameObject->DisableDrawSelf();
		gameObject->DisableDrawChildren();
	}
}

void GameObject::Draw(glm::mat4 cameraVP)
{
	glm::mat4 worldTransform = *m_worldTransform->GetMatrix();
	glm::mat4 drawTransform = cameraVP * worldTransform;
	if(HasCRender() && m_isDrawSelfEnabled)
	{
		m_render->UpdateShader(*m_localTransform->GetMatrix());
		m_render->Draw(drawTransform);
	}
	for(auto& gameObject : m_children)
	{
		gameObject->Draw(cameraVP);
	}
}

void GameObject::UpdateWorldTransform(glm::mat4 parentsMVP)
{
	glm::mat4 worldTransform = parentsMVP * (*m_localTransform->GetMatrix());
	m_worldTransform->OverrideMatrix(worldTransform);
	for(auto& gameObject : m_children)
	{
		gameObject->UpdateWorldTransform(worldTransform);
	}
}

void GameObject::OnDestroy()
{
	std::unordered_set<GameObject*> childrenCopy = m_children;

	for(GameObject* gameObject : childrenCopy)
	{
		gameObject->OnDestroy();
	}
	m_children.clear();

	if(m_parent)
	{
		m_parent->RemoveChild(this);
	}

	Uknitty::Engine::GetInstance()->RemoveGameObject(this);
	delete this;
}

void GameObject::SetParent(GameObject* parent)
{
	if(m_parent)
	{
		m_parent->RemoveChild(this);
	}
	m_parent = parent;
	m_parent->AddChild(this);
}

void GameObject::AddChild(GameObject* child)
{
	m_children.insert(child);
}

void GameObject::RemoveChild(GameObject* child)
{
	m_children.erase(child);
}

CInput* GameObject::AddCInput(CInput* cinput)
{
	if(m_input) throw std::runtime_error("GameObject already has Input");
	m_input = cinput;
	return m_input;
}

CRender* GameObject::AddCRender()
{
	if(m_render) throw std::runtime_error("GameObject already has Render");
	m_render = new CRender();
	return m_render;
}

CPhysics* GameObject::AddCPhysics()
{
	if(m_physics) throw std::runtime_error("GameObject already has Physics");
	m_physics = new CPhysics();
	return m_physics;
}

CRender* GameObject::GetCRender() const
{
	if(!m_render) throw std::runtime_error("GameObject does not have Render");
	return m_render;
}

CPhysics* GameObject::GetCPhysics() const
{
	if(!m_physics) throw std::runtime_error("GameObject does not have Physics");
	return m_physics;
}

CInput* GameObject::GetCInput() const
{
	if(!m_input) throw std::runtime_error("GameObject does not have Input");
	return m_input;
}

} // namespace Uknitty
