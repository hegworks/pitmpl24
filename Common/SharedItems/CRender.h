#pragma once

#include "glm/glm.hpp"
#include <vector>

namespace Uknitty
{

class Model;
class ShaderProgram;
class CTransform;

class CRender
{
public:
	void Initialize(Model* model, ShaderProgram* shaderProgram);
	void Draw(glm::mat4 mvp);
	void UpdateShader(glm::mat4 modelMatrix) const;
	void UpdateBonesInShader(std::vector<glm::mat4>* transformss) const;

	Model* m_model = nullptr;
	ShaderProgram* m_shaderProgram = nullptr;
};

} // namespace Uknitty
