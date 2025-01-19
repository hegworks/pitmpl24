#pragma once

#include "GameObject.h"
#include "glm/glm.hpp"
#include "LightStructs.h"

namespace Uknitty
{
class LightManager;

class LightObject : public GameObject
{
public:
	virtual void OnAwake() override;
	virtual void OnDestroy() override;

	void SetLightData(LightData* lightData);
	void SetPosition(glm::vec3 pos);
	void SetDirection(glm::vec3 dir);
	LightData* GetLightData() { return m_lightData; }

private:
	LightManager* m_lightManager = nullptr;
	LightData* m_lightData = nullptr;
};

} // namespace Uknitty