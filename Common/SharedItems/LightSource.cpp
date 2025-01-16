#include "LightSource.h"

#include "Engine.h"
#include "LightManager.h"

namespace Uknitty
{

void Uknitty::LightSource::OnAwake()
{
	m_lightManager = Engine::GetInstance()->GetLightManager();
}

void LightSource::Initialize(glm::vec3 lightColor)
{
	m_lightColor = lightColor;
	m_lightManager->NewLightSourceCreated(this, lightColor);
}

void LightSource::SetColor(glm::vec3 color)
{
	m_lightColor = color;
	m_lightManager->SetLightColor(color);
}

} // namespace Uknitty
