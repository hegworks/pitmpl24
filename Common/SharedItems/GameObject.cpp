#include "GameObject.h"

#include "CInput.h"
#include "CPhysics.h"
#include "CRender.h"
#include "CTransform.h"
#include "glm/glm.hpp"

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
}

void GameObject::Draw(glm::mat4 parentsMVP)
{
	glm::mat4 transform = parentsMVP * (*m_localTransform->GetMatrix());
	m_worldTransform->OverrideMatrix(transform);
	if(m_render)
	{
		m_render->Draw(transform);
	}
	for(auto& gameObject : m_children)
	{
		gameObject->Draw(transform);
	}
}

void GameObject::AddInput()
{
	if(m_input) throw std::runtime_error("GameObject already has Input");
	m_input = new CInput();
}

void GameObject::AddRender()
{
	if(m_render) throw std::runtime_error("GameObject already has Render");
	m_render = new CRender();
}

void GameObject::AddPhysics()
{
	if(m_physics) throw std::runtime_error("GameObject already has Physics");
	m_physics = new CPhysics();
}

CRender* GameObject::GetRender() const
{
	if(!m_render) throw std::runtime_error("GameObject does not have Render");
	return m_render;
}

CPhysics* GameObject::GetPhysics() const
{
	if(!m_physics) throw std::runtime_error("GameObject does not have Physics");
	return m_physics;
}

CInput* GameObject::GetInput() const
{
	if(!m_input) throw std::runtime_error("GameObject does not have Input");
	return m_input;
}

} // namespace Uknitty
