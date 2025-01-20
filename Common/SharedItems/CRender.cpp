#include "CRender.h"

#include "Model.h"
#include "ShaderProgram.h"
#include "ShaderType.h"
#include <string>
#include <vector>

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

void CRender::UpdateShader(glm::mat4 modelMatrix) const
{
	if(m_shaderProgram->GetShaderType() == ShaderType::LIT)
	{
		m_shaderProgram->Use();
		m_shaderProgram->SetMat4("uModel", modelMatrix);
		m_shaderProgram->SetMat4("uModelNormal", glm::transpose(glm::inverse(modelMatrix)));
		m_shaderProgram->UnUse();
	}
}

void CRender::UpdateBonesInShader(std::vector<glm::mat4>* transforms) const
{
	if(m_shaderProgram->GetShaderType() == ShaderType::LIT)
	{
		m_shaderProgram->Use();
		for(int i = 0; i < transforms->size(); i++)
		{
			m_shaderProgram->SetMat4("uFinalBonesMatrices[" + std::to_string(i) + "]", (*transforms)[i]);
		}
		m_shaderProgram->UnUse();
	}
}

} // namespace Uknitty
