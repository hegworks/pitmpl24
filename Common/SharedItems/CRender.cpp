#include "CRender.h"

#include "Model.h"
#include "ShaderProgram.h"
#include "ShaderType.h"

namespace Uknitty
{

void CRender::Initialize(Model* model, ShaderProgram* shaderProgram)
{
	m_model = model;
	m_shaderProgram = shaderProgram;
}

void CRender::Draw(glm::mat4 mvp)
{
	m_shaderProgram->Use();
	m_shaderProgram->SetMat4("uMVP", mvp);
	glDisable(GL_BLEND);
	m_model->Draw(*m_shaderProgram);
	m_shaderProgram->UnUse();
}

void CRender::UpdateShader(glm::mat4 modelMatrix)
{
	if(m_shaderProgram->GetShaderType() == ShaderType::PHONG)
	{
		m_shaderProgram->Use();
		m_shaderProgram->SetMat4("uModel", modelMatrix);
		m_shaderProgram->UnUse();
	}
}

} // namespace Uknitty
