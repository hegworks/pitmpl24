#pragma once

#include "GameObject.h"
#include "LightStructs.h"

namespace Uknitty
{
class LightManager;

class LightSource : public GameObject
{
public:
	virtual void OnAwake() override;
	virtual void OnDestroy() override;

	void SetLightData(LightData* lightData);
	LightData* GetLightData() { return m_lightData; }

private:
	LightManager* m_lightManager = nullptr;
	LightData* m_lightData = nullptr;
};

} // namespace Uknitty