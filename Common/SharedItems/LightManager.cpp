#include "LightManager.h"

#include "AssetManager.h"
#include "Engine.h"
#include "ShaderProgram.h"
#include <stdexcept>

namespace Uknitty
{

Uknitty::LightManager::LightManager(AssetManager* assetManager)
{
	m_assetManager = assetManager;
}

void LightManager::SetAmbientColor(glm::vec3 color)
{
	m_ambientColor = color;
	auto& allShaderPrograms = m_assetManager->GetAllShaderPrograms();
	for(auto& [shaderType, shaderProgram] : allShaderPrograms)
	{
		switch(shaderType)
		{
			case ShaderType::DEFAULT:
			case ShaderType::BULLET_PHYSICS:
				break;
			case ShaderType::UNLIT:
				break;
			case ShaderType::PHONG:
				shaderProgram->Use();
				shaderProgram->SetVec3("uAmbientColor", m_ambientColor);
				shaderProgram->UnUse();
				break;
			default:
				throw std::runtime_error("invalid shaderType");
		}
	}
}

void LightManager::SetAmbientStrength(float strength)
{
	m_ambientStrength = strength;
	auto& allShaderPrograms = m_assetManager->GetAllShaderPrograms();
	for(auto& [shaderType, shaderProgram] : allShaderPrograms)
	{
		switch(shaderType)
		{
			case ShaderType::DEFAULT:
			case ShaderType::BULLET_PHYSICS:
				break;
			case ShaderType::UNLIT:
				break;
			case ShaderType::PHONG:
				shaderProgram->Use();
				shaderProgram->SetFloat("uAmbientStrength", m_ambientStrength);
				shaderProgram->UnUse();
				break;
			default:
				throw std::runtime_error("invalid shaderType");
		}
	}
}

} // namespace Uknitty
