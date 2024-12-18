#include "DynamicObject.h"

#include "glm/glm.hpp"
#include "ICamera.h"
#include "Model.h"
#include "Physics.h"
#include "SharedDependencies.h"
#include "Transform.h"

DynamicObject::~DynamicObject()
{
	m_btDynamicsWorld->removeRigidBody(m_physics->GetRigidBody());
	std::cout << "Removing RigidBody: " << m_model->GetStrippedFileName() << " remaining: " << m_btDynamicsWorld->getNumCollisionObjects() << std::endl;
	delete m_transform;
}

void DynamicObject::Construct(Uknitty::Model* model, glm::vec3 position, Uknitty::Physics* physics)
{
	m_iCamera = SharedDependencies::GetCamera();
	m_btDynamicsWorld = SharedDependencies::GetDynamicsWorld();
	m_shaderProgram = SharedDependencies::GetShaderProgram();

	m_model = model;
	m_physics = physics;

	m_transform = new Uknitty::Transform();
	m_transform->SetPosition(position);
}

void DynamicObject::SetPosition(glm::vec3 pos)
{
	m_physics->SetPosition(pos);
}

void DynamicObject::Awake()
{
}

void DynamicObject::Start()
{
}

void DynamicObject::Update(float deltaTime)
{
	SetModelPosToPhysicsPos();
}

void DynamicObject::LateUpdate(float deltaTime)
{
}

void DynamicObject::FixedUpdate()
{
}

void DynamicObject::Destroy()
{
	std::cout << "Destroying DynamicObject with model: " << m_model->GetStrippedFileName() << std::endl;
	delete this;
}

void DynamicObject::Draw()
{
	m_shaderProgram->Use();
	m_shaderProgram->SetMat4("uView", m_iCamera->GetView());
	m_shaderProgram->SetMat4("uProjection", m_iCamera->GetProjection());
	m_shaderProgram->SetMat4("uModel", *m_transform->GetModelMatrix());
	glDisable(GL_BLEND);
	m_model->Draw(*m_shaderProgram);
	m_shaderProgram->UnUse();
}

glm::vec3 DynamicObject::GetCurrentPhysicsPos()
{
	return Uknitty::Physics::BtVec3ToGLMVec3(m_physics->GetRigidBody()->getWorldTransform().getOrigin());
}

void DynamicObject::SetModelPosToPhysicsPos()
{
	m_transform->SetPosition(GetCurrentPhysicsPos());
}
