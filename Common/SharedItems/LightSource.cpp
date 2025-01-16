#include "LightSource.h"

#include "Engine.h"
#include "LightManager.h"
#include "LightStructs.h"

namespace Uknitty
{

void Uknitty::LightSource::OnAwake()
{
	m_lightManager = Engine::GetInstance()->GetLightManager();
	m_lightManager->NewLightSourceCreated(this);
}

void LightSource::OnDestroy()
{
	m_lightManager->LightSourceDestroyed(this);
}

void LightSource::SetLightData(LightData* lightData)
{
	m_lightData = lightData;
	m_lightManager->SetLightData(GameObject::GetID(), lightData);
}

} // namespace Uknitty
