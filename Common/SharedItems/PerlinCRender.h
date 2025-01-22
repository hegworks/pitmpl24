#pragma once

#include "CRender.h"
#include <iostream>

namespace Uknitty
{
class ShaderProgram;
}

class PerlinCRender : public Uknitty::CRender
{
public:
	PerlinCRender(Uknitty::ShaderProgram* perlinShader);
	~PerlinCRender() = default;

	virtual void Draw(glm::mat4 mvp) override;
	void SetTextureId(unsigned int textureId) { m_textureId = textureId; }
	void SetVAO(unsigned int vao)
	{
		m_vao = vao;
		std::cout << "vao: " << m_vao << std::endl;
	}
	void SetIndexCount(unsigned int indexCount) { m_indexCount = indexCount; }
	void SetIsMeshTrueIsTextureFalse(bool isMeshTrueIsTextureFalse) { m_isMeshTrueIsTextureFalse = isMeshTrueIsTextureFalse; }

private:
	unsigned int m_textureId = 0;
	unsigned int m_vao = 0;
	unsigned int m_indexCount = 0;
	bool m_isMeshTrueIsTextureFalse = false;
};
