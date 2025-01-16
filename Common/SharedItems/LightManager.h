#pragma once

#include "glm/glm.hpp"
#include "LightStructs.h"
#include <string>
#include <vector>

namespace Uknitty
{
class Engine;
class AssetManager;
class LightSource;
class ShaderProgram;

class LightManager
{
public:
	LightManager(AssetManager* assetManager);

	struct GlobalProperties
	{
		inline static const std::string AMBIENT_STRENGTH = "global.ambientStrength";
		inline static const std::string AMBIENT_COLOR = "global.ambientColor";
		inline static const std::string VIEW_POS = "global.viewPos";
	};

	struct LightProperties
	{
		inline static const std::string POS = "light.pos";
		inline static const std::string DIFFUSE_COLOR = "light.diffuseColor";
		inline static const std::string SPECULAR_COLOR = "light.specularColor";
		inline static const std::string SPECULAR_STRENGTH = "light.specularStrength";
		inline static const std::string SHININESS = "light.shininess";
	};

	struct DirLightProperties
	{
		inline static const std::string DIRECTION = "dirLight.direction";
		inline static const std::string DIFFUSE_COLOR = "dirLight.diffuseColor";
		inline static const std::string SPECULAR_COLOR = "dirLight.specularColor";
		inline static const std::string SPECULAR_STRENGTH = "dirLight.specularStrength";
		inline static const std::string SHININESS = "dirLight.shininess";
	};

	void Update(float deltaTime);
	void SetAmbientColor(glm::vec3 color);
	void SetAmbientStrength(float strength);
	void NewLightSourceCreated(LightSource* lightSource);
	void LightSourceDestroyed(LightSource* lightSource);
	void SetLightData(LightData* lightData);
	void SetDirectionalLightData(DirLightData* dirLightData);
	void SetUnlitColor(glm::vec3 color);
	DirLightData* GetDirLightData() { return m_dirLightData; }

private:
	AssetManager* m_assetManager = nullptr;
	std::vector<LightSource*> m_lightSources;
	ShaderProgram* m_phong = nullptr;
	DirLightData* m_dirLightData = nullptr;

	const int MAX_LIGHTS = 1;

	int m_createdLights = 0;
	glm::vec3 m_ambientColor = glm::vec3(1.0);
	float m_ambientStrength = 1.0;
};

} // namespace Uknitty
