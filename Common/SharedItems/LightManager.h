#pragma once

#include "glm/glm.hpp"
#include <vector>

namespace Uknitty
{
class Engine;
class AssetManager;
class LightSource;

class LightManager
{
public:
	LightManager(AssetManager* assetManager);

	void Update(float deltaTime);
	void SetAmbientColor(glm::vec3 color);
	void SetAmbientStrength(float strength);
	void NewLightSourceCreated(LightSource* lightSource, glm::vec3 lightColor);
	void SetLightColor(glm::vec3 Color);

private:
	AssetManager* m_assetManager = nullptr;
	std::vector<LightSource*> m_lightSources;

	const int MAX_LIGHTS = 1;

	glm::vec3 m_ambientColor = glm::vec3(1.0);
	float m_ambientStrength = 1.0;
	int m_createdLights = 0;
};

} // namespace Uknitty
