#include "ICamera.h"
#include "Model.h"
#include "Physics.h"
#include "ShaderProgram.h"
#include "SolidObject.h"
#include "Transform.h"
#include <iostream>

SolidObject::SolidObject(Uknitty::ICamera* iCamera, Uknitty::Model* model, Uknitty::ShaderProgram* shaderProgram, btDynamicsWorld* btDynamicsWorld, glm::vec3 modelDimensions, glm::vec3 position)
{
	SetDependencies(iCamera, model, shaderProgram, btDynamicsWorld);

	m_transform = new Uknitty::Transform();
	m_transform->SetPosition(position);

	m_physics = new Uknitty::Physics();
	m_physics->InitialzeWithBoxShape(position, modelDimensions, 0);
	auto userPointerData = new Uknitty::Physics::UserPointerData();
	userPointerData->physicsType = Uknitty::Physics::PhysicsType::SOLID;
	userPointerData->name = model->GetStrippedFileName();
	m_physics->SetUserPointerData(userPointerData);
	m_btDynamicsWorld->DebugAddRigidBody(m_physics->GetRigidBody(), model->GetStrippedFileName());
}

SolidObject::~SolidObject()
{
	m_btDynamicsWorld->DebugRemoveRigidBody(m_physics->GetRigidBody(), m_model->GetStrippedFileName());
	delete m_physics;
	delete m_transform;
}

void SolidObject::Draw()
{
	m_shaderProgram->Use();
	m_shaderProgram->SetMat4("uView", m_iCamera->GetView());
	m_shaderProgram->SetMat4("uProjection", m_iCamera->GetProjection());
	m_shaderProgram->SetMat4("uModel", *m_transform->GetModelMatrix());
	glDisable(GL_BLEND);
	m_model->Draw(*m_shaderProgram);
	m_shaderProgram->UnUse();
}

void SolidObject::Destroy()
{
	std::cout << "Destroying SolidObject with model: " << m_model->GetStrippedFileName() << std::endl;
	delete this;
}

void SolidObject::SetDependencies(Uknitty::ICamera* iCamera, Uknitty::Model* model, Uknitty::ShaderProgram* shaderProgram, btDynamicsWorld* btDynamicsWorld)
{
	m_iCamera = iCamera;
	m_model = model;
	m_shaderProgram = shaderProgram;
	m_btDynamicsWorld = btDynamicsWorld;
}
