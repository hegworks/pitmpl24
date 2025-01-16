#pragma once

#include "GameObject.h"

namespace Uknitty
{
class LightManager;


class LightSource : public GameObject
{
public:
	virtual void OnAwake() override;

	void Initialize(glm::vec3 lightColor);
	void SetColor(glm::vec3 color);

private:
	LightManager* m_lightManager = nullptr;
	glm::vec3 m_lightColor = glm::vec3(1);
};

} // namespace Uknitty