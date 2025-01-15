#pragma once

#include "glm/glm.hpp"
#include "ShaderType.h"
#include <string>
#include <unordered_map>

namespace Uknitty
{

class Model;
class ShaderProgram;

class AssetManager
{
public:
	~AssetManager();

	Model* AutoGetModel(std::string key, std::string filePath = "", glm::vec2 textureCoordScale = glm::vec2(1), bool shouldVerticallyFlipTexture = false);
	ShaderProgram* AutoGetShaderProgram(ShaderType shaderType, std::string vertexShaderPath = "", std::string fragmentShaderPath = "");
	const std::unordered_map<ShaderType, ShaderProgram*>& GetAllShaderPrograms()  const { return m_shaderPrograms; }

private:
	std::unordered_map<std::string, Model*> m_models;
	std::unordered_map<ShaderType, ShaderProgram*> m_shaderPrograms;
};

} // namespace Uknitty
