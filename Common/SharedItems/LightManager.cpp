#include "LightManager.h"

#include "AssetManager.h"
#include "CameraObject.h"
#include "CTransform.h"
#include "Engine.h"
#include "GameObject.h"
#include "GlCheckError.h"
#include "LightSource.h"
#include "ShaderProgram.h"
#include "ShaderType.h"
#include <stdexcept>

namespace Uknitty
{

Uknitty::LightManager::LightManager(AssetManager* assetManager)
{
	m_assetManager = assetManager;
}

void LightManager::Update(float deltaTime)
{
	ShaderProgram* phong = m_assetManager->AutoGetShaderProgram(ShaderType::PHONG);
	glm::vec3 cameraPos = *Engine::GetInstance()->GetMainCamera()->GetWorldTransform()->GetPosition();
	for(auto& lightSource : m_lightSources)
	{
		phong->Use();
		phong->SetVec3("uLightPos", *lightSource->GetWorldTransform()->GetPosition());
		phong->SetVec3("uViewPos", cameraPos);
		phong->UnUse();
	}
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

void LightManager::NewLightSourceCreated(LightSource* lightSource, glm::vec3 lightColor)
{
	m_createdLights++;
	if(m_createdLights > MAX_LIGHTS)
	{
		throw std::runtime_error("Maximum number of lights reached");
	}

	ShaderProgram* phong = m_assetManager->AutoGetShaderProgram(ShaderType::PHONG);
	phong->Use();
	phong->SetVec3("uLightColor", lightColor);
	phong->UnUse();
	m_lightSources.push_back(lightSource);
}

void LightManager::SetLightColor(glm::vec3 Color)
{
	ShaderProgram* phong = m_assetManager->AutoGetShaderProgram(ShaderType::PHONG);
	phong->Use();
	phong->SetVec3("uLightColor", Color);
	phong->UnUse();
}

} // namespace Uknitty
