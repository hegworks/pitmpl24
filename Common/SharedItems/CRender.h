#pragma once

#include "glm/glm.hpp"

namespace Uknitty
{

class Model;
class ShaderProgram;
class CTransform;

class CRender
{
public:
	void Draw(glm::mat4 mvp);

	Model* m_model = nullptr;
	ShaderProgram* m_shaderProgram = nullptr;
};

} // namespace Uknitty
