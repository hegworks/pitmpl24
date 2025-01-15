#include "AssetManager.h"

#include "Model.h"
#include "ShaderProgram.h"
#include "ShaderType.h"
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace Uknitty
{

AssetManager::~AssetManager()
{
	for(auto& [key, model] : m_models)
	{
		delete model;
	}
	m_models.clear();

	for(auto& [key, shaderProgram] : m_shaderPrograms)
	{
		delete shaderProgram;
	}
	m_shaderPrograms.clear();
}

Model* AssetManager::AutoGetModel(std::string key, std::string filePath, glm::vec2 textureCoordScale, bool shouldVerticallyFlipTexture)
{
	if(m_models.find(key) != m_models.end())
	{
		return m_models[key];
	}
	if(filePath == "")
	{
		throw std::runtime_error("Model with key does not exist and filePath is empty");
	}
	m_models[key] = new Model(filePath, textureCoordScale, shouldVerticallyFlipTexture);
	return m_models[key];
}

ShaderProgram* AssetManager::AutoGetShaderProgram(ShaderType shaderType, std::string vertexShaderPath, std::string fragmentShaderPath)
{
	if(m_shaderPrograms.find(shaderType) != m_shaderPrograms.end())
	{
		return m_shaderPrograms[shaderType];
	}
	if(vertexShaderPath == "" || fragmentShaderPath == "")
	{
		throw std::runtime_error("ShaderProgram with key does not exist and vertexShaderPath or fragmentShaderPath is empty");
	}
	m_shaderPrograms[shaderType] = new ShaderProgram(vertexShaderPath, fragmentShaderPath, shaderType);
	return m_shaderPrograms[shaderType];
}

} // namespace Uknitty
