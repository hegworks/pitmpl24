#include "LightManager.h"

#include "AssetManager.h"
#include "CameraObject.h"
#include "CTransform.h"
#include "Engine.h"
#include "GameObject.h"
#include "GlCheckError.h"
#include "LightSource.h"
#include "LightStructs.h"
#include "ShaderProgram.h"
#include "ShaderType.h"
#include <stdexcept>

namespace Uknitty
{

Uknitty::LightManager::LightManager(AssetManager* assetManager)
{
	m_assetManager = assetManager;
	m_phong = m_assetManager->AutoGetShaderProgram(Uknitty::ShaderType::PHONG, "../Common/Assets/Shaders/PhongVertex.glsl", "../Common/Assets/Shaders/PhongFragment.glsl");
}

void LightManager::Update(float deltaTime)
{
	glm::vec3 cameraPos = *Engine::GetInstance()->GetMainCamera()->GetWorldTransform()->GetPosition();

	for(auto& lightSource : m_lightSources)
	{
		m_phong->Use();

		m_phong->SetVec3(LightProperties::POS, *lightSource->GetWorldTransform()->GetPosition());
		m_phong->SetVec3(GlobalProperties::VIEW_POS, cameraPos);

		if(lightSource->GetLightData()->lightType == LightType::DIR_LIGHT ||
		   lightSource->GetLightData()->lightType == LightType::SPOT_LIGHT)
		{
			m_phong->SetVec3(LightProperties::DIRECTION, lightSource->GetWorldTransform()->GetForward());
		}

		m_phong->UnUse();
	}
}

void LightManager::SetAmbientColor(glm::vec3 color)
{
	m_ambientColor = color;
	m_phong->Use();
	m_phong->SetVec3(GlobalProperties::AMBIENT_COLOR, m_ambientColor);
	m_phong->UnUse();
}

void LightManager::SetAmbientStrength(float strength)
{
	m_ambientStrength = strength;
	m_phong->Use();
	m_phong->SetFloat(GlobalProperties::AMBIENT_STRENGTH, m_ambientStrength);
	m_phong->UnUse();
}

void LightManager::NewLightSourceCreated(LightSource* lightSource)
{
	m_createdLights++;
	if(m_createdLights > MAX_LIGHTS)
	{
		throw std::runtime_error("Maximum number of lights reached");
	}

	m_lightSources.push_back(lightSource);
}

void LightManager::LightSourceDestroyed(LightSource* lightSource)
{
	m_createdLights--;
	m_lightSources.erase(std::remove(m_lightSources.begin(), m_lightSources.end(), lightSource), m_lightSources.end());
}

void LightManager::SetLightData(LightData* lightData)
{
	m_phong->Use();

	m_phong->SetInt(LightProperties::TYPE, static_cast<int>(lightData->lightType));

	m_phong->SetVec3(LightProperties::DIFFUSE_COLOR, lightData->diffuseColor);
	m_phong->SetVec3(LightProperties::SPECULAR_COLOR, lightData->specularColor);
	m_phong->SetFloat(LightProperties::SPECULAR_STRENGTH, lightData->specularStrength);
	m_phong->SetFloat(LightProperties::SHININESS, lightData->shininess);

	if(lightData->lightType == LightType::POINT_LIGHT || lightData->lightType == LightType::SPOT_LIGHT)
	{
		m_phong->SetFloat(LightProperties::ATT_CONST, lightData->attConst);
		m_phong->SetFloat(LightProperties::ATT_LIN, lightData->attLin);
		m_phong->SetFloat(LightProperties::ATT_QUAD, lightData->attQuad);
	}

	if(lightData->lightType == LightType::SPOT_LIGHT)
	{
		m_phong->SetFloat(LightProperties::SPOT_CUTOFF, glm::cos(glm::radians(lightData->cutOff)));
		m_phong->SetFloat(LightProperties::SPOT_OUTER_CUTOFF, glm::cos(glm::radians(lightData->outerCutOff)));
	}

	m_phong->UnUse();
}

//void LightManager::SetDirectionalLightData(DirLightData* dirLightData)
//{
//	m_dirLightData = dirLightData;
//	m_phong->Use();
//	m_phong->SetVec3(DirLightProperties::DIRECTION, dirLightData->direction);
//	m_phong->SetVec3(DirLightProperties::DIFFUSE_COLOR, dirLightData->diffuseColor);
//	m_phong->SetVec3(DirLightProperties::SPECULAR_COLOR, dirLightData->specularColor);
//	m_phong->SetFloat(DirLightProperties::SPECULAR_STRENGTH, dirLightData->specularStrength);
//	m_phong->SetFloat(DirLightProperties::SHININESS, dirLightData->shininess);
//	m_phong->UnUse();
//}

void LightManager::SetUnlitColor(glm::vec3 color)
{
	ShaderProgram* unlit = m_assetManager->AutoGetShaderProgram(Uknitty::ShaderType::UNLIT);
	unlit->Use();
	unlit->SetVec4("uColor", glm::vec4(color, 1));
	unlit->UnUse();
}

} // namespace Uknitty
