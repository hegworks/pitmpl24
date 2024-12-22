#include "SolidObject.h"

#include "Common.h"
#include "ICamera.h"
#include "Model.h"
#include "Physics.h"
#include "PhysicsCollisionFilters.h"
#include "ShaderProgram.h"
#include "SharedDependencies.h"
#include "Transform.h"
#include <iostream>

SolidObject::SolidObject(Uknitty::Model* model, glm::vec3 modelDimensions, glm::vec3 position)
{
	m_btDynamicsWorld = SharedDependencies::GetDynamicsWorld();
	m_iCamera = SharedDependencies::GetCamera();
	m_shaderProgram = SharedDependencies::GetShaderProgram();

	m_model = model;

	m_transform = new Uknitty::Transform();
	m_transform->SetPosition(position);

	m_physics = new Uknitty::Physics();
	m_physics->InitialzeWithBoxShape(position, modelDimensions, 0);

	m_btDynamicsWorld->addRigidBody(m_physics->GetRigidBody(), COLL_GROUP_OBSTACLE, COLL_MASK_OBSTACLE);
	std::cout << "Adding RigidBody: " << model->GetStrippedFileName() << std::endl;
}

SolidObject::~SolidObject()
{
	m_btDynamicsWorld->removeRigidBody(m_physics->GetRigidBody());
	std::cout << "Removing RigidBody: " << m_model->GetStrippedFileName() << " remaining: " << m_btDynamicsWorld->getNumCollisionObjects() << std::endl;
	delete m_physics;
	delete m_transform;
}

void SolidObject::Draw()
{
#ifndef DEBUG_DONT_DRAW_SOLIDS
	m_shaderProgram->Use();
	m_shaderProgram->SetMat4("uView", m_iCamera->GetView());
	m_shaderProgram->SetMat4("uProjection", m_iCamera->GetProjection());
	m_shaderProgram->SetMat4("uModel", *m_transform->GetModelMatrix());
	glDisable(GL_BLEND);
	m_model->Draw();
	m_shaderProgram->UnUse();
#endif // DEBUG_DONT_DRAW_SOLIDS
}

void SolidObject::Destroy()
{
	std::cout << "Destroying SolidObject with model: " << m_model->GetStrippedFileName() << std::endl;
	delete this;
}
