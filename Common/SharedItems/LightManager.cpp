#include "LightManager.h"

#include "AssetManager.h"
#include "CameraObject.h"
#include "CTransform.h"
#include "Engine.h"
#include "GameObject.h"
#include "GlCheckError.h"
#include "LightObject.h"
#include "LightStructs.h"
#include "ShaderProgram.h"
#include "ShaderType.h"
#include <stdexcept>
#include <string>

namespace Uknitty
{

Uknitty::LightManager::LightManager(AssetManager* assetManager)
{
	m_assetManager = assetManager;
#ifdef WINDOWS_BUILD
	m_isWindows = true;

	m_lit = assetManager->AutoGetShaderProgram(Uknitty::ShaderType::LIT,
											   "../Common/Assets/Shaders/Win/LitVertex.glsl",
											   "../Common/Assets/Shaders/Win/LitFragment.glsl");
#endif // WINDOWS_BUILD
#ifdef Raspberry_BUILD
	m_isWindows = false;

	{ // keep only one of these 3 lines in the #if 1
#if 1
		m_lit = assetManager->AutoGetShaderProgram(Uknitty::ShaderType::LIT, "../Common/Assets/Shaders/Pi/LitVertex_Hybrid.glsl", "../Common/Assets/Shaders/Pi/LitFragment_Hybrid.glsl");
#endif // 1
#if 0
		m_lit = assetManager->AutoGetShaderProgram(Uknitty::ShaderType::LIT, "../Common/Assets/Shaders/Pi/LitVertex_FullGourand.glsl", "../Common/Assets/Shaders/Pi/LitFragment_FullGourand.glsl");
		m_lit = assetManager->AutoGetShaderProgram(Uknitty::ShaderType::LIT, "../Common/Assets/Shaders/Pi/LitVertex_FullPhong.glsl", "../Common/Assets/Shaders/Pi/LitFragment_FullPhong.glsl");
#endif // 0
	}

#endif // Raspberry_BUILD
}

void LightManager::Update(float deltaTime)
{
	glm::vec3 cameraPos = *Engine::GetInstance()->GetMainCamera()->GetWorldTransform()->GetPosition();

	for(auto& [id, lightSource] : m_lightSources)
	{
		std::string prefix = CalculatePrefix(id);
		LightType lightType = lightSource->GetLightData()->lightType;

		m_lit->Use();

		m_lit->SetVec3(GlobalProperties::VIEW_POS, cameraPos);

		if(lightSource->GetLightData()->isAutoUpdate)
		{
			m_lit->SetVec3(prefix + LightProperties::POS, *lightSource->GetWorldTransform()->GetPosition());

			if(lightType == LightType::DIR_LIGHT ||
			   lightType == LightType::SPOT_LIGHT)
			{
				m_lit->SetVec3(prefix + LightProperties::DIRECTION, lightSource->GetWorldTransform()->GetForward());
			}
		}

		m_lit->UnUse();
	}
}

void LightManager::SetAmbientColor(glm::vec3 color)
{
	m_ambientColor = color;
	m_lit->Use();
	m_lit->SetVec3(GlobalProperties::AMBIENT_COLOR, m_ambientColor);
	m_lit->UnUse();
}

void LightManager::SetAmbientStrength(float strength)
{
	m_ambientStrength = strength;
	m_lit->Use();
	m_lit->SetFloat(GlobalProperties::AMBIENT_STRENGTH, m_ambientStrength);
	m_lit->UnUse();
}

void LightManager::NewLightSourceCreated(LightObject* lightSource)
{
	if(m_lightSources.size() + 1 > MAX_LIGHTS)
	{
		throw std::runtime_error("Maximum number of lights reached");
	}

	m_lightSources[lightSource->GetID()] = lightSource;
	m_idToIndex[lightSource->GetID()] = m_lightSources.size() - 1;

	m_lit->Use();
	m_lit->SetInt(GlobalProperties::LIGHTS_COUNT, m_lightSources.size());
	m_lit->UnUse();
}

void LightManager::LightSourceDestroyed(LightObject* lightSource)
{
	m_lightSources.erase(lightSource->GetID());
}

void LightManager::SetLightData(int id, LightData* lightData)
{
	m_lit->Use();

	std::string prefix = CalculatePrefix(id);

	m_lit->SetInt(prefix + LightProperties::TYPE, static_cast<int>(lightData->lightType));

	m_lit->SetVec3(prefix + LightProperties::DIFFUSE_COLOR, lightData->diffuseColor);
	if(m_isWindows)
	{
		m_lit->SetVec3(prefix + LightProperties::SPECULAR_COLOR, lightData->specularColor);
		m_lit->SetFloat(prefix + LightProperties::SPECULAR_STRENGTH, lightData->specularStrength);
		m_lit->SetFloat(prefix + LightProperties::SHININESS, lightData->shininess);
	}

	if(lightData->lightType == LightType::POINT_LIGHT || lightData->lightType == LightType::SPOT_LIGHT)
	{
		m_lit->SetFloat(prefix + LightProperties::ATT_CONST, lightData->attConst);
		m_lit->SetFloat(prefix + LightProperties::ATT_LIN, lightData->attLin);
		m_lit->SetFloat(prefix + LightProperties::ATT_QUAD, lightData->attQuad);
	}

	if(lightData->lightType == LightType::SPOT_LIGHT)
	{
		m_lit->SetFloat(prefix + LightProperties::SPOT_CUTOFF, glm::cos(glm::radians(lightData->cutOff)));
		if(m_isWindows)
		{
			m_lit->SetFloat(prefix + LightProperties::SPOT_OUTER_CUTOFF, glm::cos(glm::radians(lightData->outerCutOff)));
		}
	}

	if(lightData->lightType == LightType::DIR_LIGHT ||
	   lightData->lightType == LightType::SPOT_LIGHT)
	{
		m_lit->SetVec3(prefix + LightProperties::DIRECTION, lightData->direction);
	}

	m_lit->SetVec3(prefix + LightProperties::POS, lightData->position);

	if(lightData->lightType == LightType::DIR_LIGHT)
	{
		m_lit->SetVec3(prefix + LightProperties::DIRECTION, lightData->direction);
	}

	m_lit->UnUse();
}

void LightManager::SetUnlitColor(glm::vec3 color)
{
	ShaderProgram* unlit = m_assetManager->AutoGetShaderProgram(Uknitty::ShaderType::UNLIT);
	unlit->Use();
	unlit->SetVec4("uColor", glm::vec4(color, 1));
	unlit->UnUse();
}

void LightManager::SetPosition(int id, glm::vec3 pos)
{
	std::string prefix = CalculatePrefix(id);
	m_lit->Use();
	m_lit->SetVec3(prefix + LightProperties::POS, pos);
	m_lit->UnUse();
}

void LightManager::SetDirection(int id, glm::vec3 dir)
{
	std::string prefix = CalculatePrefix(id);
	m_lit->Use();
	m_lit->SetVec3(prefix + LightProperties::DIRECTION, dir);
	m_lit->UnUse();
}

std::string Uknitty::LightManager::CalculatePrefix(int id)
{
	return LIGHTS_ARRAY + "[" + std::to_string(m_idToIndex[id]) + "].";
}

} // namespace Uknitty
