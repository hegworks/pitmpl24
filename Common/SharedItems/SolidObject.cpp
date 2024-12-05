#include "ICamera.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "SolidObject.h"
#include "Transform.h"

SolidObject::SolidObject(Uknitty::ICamera* iCamera, Uknitty::Model* model, Uknitty::ShaderProgram* shaderProgram)
{
	m_iCamera = iCamera;
	m_model = model;
	m_shaderProgram = shaderProgram;
	m_transform = new Uknitty::Transform();
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
