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

void LightObject::SetPosition(glm::vec3 pos)
{
	m_lightData->position = pos;
	m_lightManager->SetPosition(GameObject::GetID(), pos);
}

void LightObject::SetDirection(glm::vec3 dir)
{
	m_lightData->direction = dir;
	m_lightManager->SetDirection(GameObject::GetID(), dir);
}

} // namespace Uknitty
