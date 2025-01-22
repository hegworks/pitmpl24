#include "PerlinCRender.h"

#include "CRender.h"
#include "glm/glm.hpp"
#include "ShaderProgram.h"

PerlinCRender::PerlinCRender(Uknitty::ShaderProgram* perlinShader)
{
	m_shaderProgram = perlinShader;
}

void PerlinCRender::Draw(glm::mat4 mvp)
{
	m_shaderProgram->Use();

	m_shaderProgram->SetMat4("uMVP", mvp);

	m_shaderProgram->SetBool("uIsMeshTrueIsTextureFalse", m_isMeshTrueIsTextureFalse);
	glBindVertexArray(m_vao);
	if(m_isMeshTrueIsTextureFalse)
	{
		glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_textureId);
		// Set texture uniform
		m_shaderProgram->SetInt("uNoiseTexture", 0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

	m_shaderProgram->UnUse();
}
