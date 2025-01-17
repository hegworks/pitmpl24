#include "LightObject.h"

#include "Engine.h"
#include "LightManager.h"
#include "LightStructs.h"

namespace Uknitty
{

void Uknitty::LightObject::OnAwake()
{
	m_lightManager = Engine::GetInstance()->GetLightManager();
	m_lightManager->NewLightSourceCreated(this);
}

void LightObject::OnDestroy()
{
	m_lightManager->LightSourceDestroyed(this);
}

void LightObject::SetLightData(LightData* lightData)
{
	m_lightData = lightData;
	m_lightManager->SetLightData(GameObject::GetID(), lightData);
}

} // namespace Uknitty
