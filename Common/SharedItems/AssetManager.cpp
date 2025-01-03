#include "AssetManager.h"

#include "Model.h"
#include "ShaderProgram.h"
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace Uknitty
{

Model* AssetManager::CreateModel(std::string key, std::string filePath, glm::vec2 textureCoordScale, bool shouldVerticallyFlipTexture)
{
	if(m_models.find(key) != m_models.end())
	{
		throw std::runtime_error("Model with key already exists");
	}
	m_models[key] = new Model(filePath, textureCoordScale, shouldVerticallyFlipTexture);
	return m_models[key];
}

Model* AssetManager::GetModel(std::string key)
{
	if(m_models.find(key) == m_models.end())
	{
		throw std::runtime_error("Model with key does not exist");
	}
	return m_models[key];
}

ShaderProgram* AssetManager::CreateShaderProgram(std::string key, std::string vertexShaderPath, std::string fragmentShaderPath)
{
	if(m_shaderPrograms.find(key) != m_shaderPrograms.end())
	{
		throw std::runtime_error("ShaderProgram with key already exists");
	}
	m_shaderPrograms[key] = new ShaderProgram(vertexShaderPath, fragmentShaderPath);
	return m_shaderPrograms[key];
}

ShaderProgram* AssetManager::GetShaderProgram(std::string key)
{
	if(m_shaderPrograms.find(key) == m_shaderPrograms.end())
	{
		throw std::runtime_error("ShaderProgram with key does not exist");
	}
	return m_shaderPrograms[key];
}

} // namespace Uknitty
