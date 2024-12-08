#include "ICamera.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "SolidObject.h"
#include "Transform.h"
#include <iostream>

SolidObject::SolidObject(Uknitty::ICamera* iCamera, Uknitty::Model* model, Uknitty::ShaderProgram* shaderProgram)
{
	m_iCamera = iCamera;
	m_model = model;
	m_shaderProgram = shaderProgram;
	m_transform = new Uknitty::Transform();
}

SolidObject::~SolidObject()
{
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
	std::cout << "Destroying SolidObject with model: " << m_model->GetFileName() << std::endl;
	delete this;
}
