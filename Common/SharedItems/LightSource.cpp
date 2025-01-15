#include "LightSource.h"

#include "Engine.h"
#include "LightManager.h"

namespace Uknitty
{

void Uknitty::LightSource::OnAwake()
{
	m_lightManager = Engine::GetInstance()->GetLightManager();
	m_lightManager->NewLightSourceCreated(this);
}

} // namespace Uknitty
