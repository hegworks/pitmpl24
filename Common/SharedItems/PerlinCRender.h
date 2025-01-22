#pragma once

#include "CRender.h"

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
	void SetVAO(unsigned int vao) { m_vao = vao; }

private:
	unsigned int m_textureId = 0;
	unsigned int m_vao = 0;
};
