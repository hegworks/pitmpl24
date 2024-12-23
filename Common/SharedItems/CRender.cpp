#include "CRender.h"

#include "Model.h"
#include "ShaderProgram.h"

namespace Uknitty
{

void CRender::Draw(glm::mat4 mvp)
{
	m_shaderProgram->Use();
	m_shaderProgram->SetMat4("uMVP", mvp);
	glDisable(GL_BLEND);
	m_model->Draw();
	m_shaderProgram->UnUse();
}

} // namespace Uknitty
