#pragma once

#include "glm/glm.hpp"

namespace Uknitty
{
class Engine;
class AssetManager;

class LightManager
{
public:
	LightManager(AssetManager* assetManager);

	void SetAmbientColor(glm::vec3 color);
	void SetAmbientStrength(float strength);

private:
	AssetManager* m_assetManager = nullptr;

	glm::vec3 m_ambientColor = glm::vec3(1.0);
	float m_ambientStrength = 1.0;
};

} // namespace Uknitty
