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
	if(HasCRender())
	{
		m_render->Draw(transform);
	}
	for(auto& gameObject : m_children)
	{
		gameObject->Draw(transform);
	}
}

void GameObject::AddCInput(CInput* cinput)
{
	if(m_input) throw std::runtime_error("GameObject already has Input");
	m_input = cinput;
}

void GameObject::AddCRender()
{
	if(m_render) throw std::runtime_error("GameObject already has Render");
	m_render = new CRender();
}

void GameObject::AddCPhysics()
{
	if(m_physics) throw std::runtime_error("GameObject already has Physics");
	m_physics = new CPhysics();
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
