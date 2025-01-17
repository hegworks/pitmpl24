#pragma once

#include "glm/glm.hpp"
#include "LightStructs.h"
#include <string>
#include <unordered_map>

namespace Uknitty
{
class Engine;
class AssetManager;
class LightObject;
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
		inline static const std::string LIGHTS_COUNT = "global.lightsCount";
	};

	struct LightProperties
	{
		inline static const std::string TYPE = "type";

		inline static const std::string DIFFUSE_COLOR = "diffuseColor";
		inline static const std::string SPECULAR_COLOR = "specularColor";
		inline static const std::string SPECULAR_STRENGTH = "specularStrength";
		inline static const std::string SHININESS = "shininess";

		inline static const std::string POS = "pos";
		inline static const std::string DIRECTION = "dir";

		inline static const std::string ATT_CONST = "attConst";
		inline static const std::string ATT_LIN = "attLin";
		inline static const std::string ATT_QUAD = "attQuad";

		inline static const std::string SPOT_CUTOFF = "cutOff";
		inline static const std::string SPOT_OUTER_CUTOFF = "outerCutOff";
	};

	inline static const std::string LIGHTS_ARRAY = "lights";
	inline static const std::string LIGHTS_COUNT = "lights";

	void Update(float deltaTime);
	void SetAmbientColor(glm::vec3 color);
	void SetAmbientStrength(float strength);
	void NewLightSourceCreated(LightObject* lightSource);
	void LightSourceDestroyed(LightObject* lightSource);
	void SetLightData(int id, LightData* lightData);
	void SetUnlitColor(glm::vec3 color);

private:
	AssetManager* m_assetManager = nullptr;
	std::unordered_map<int, LightObject*> m_lightSources;
	std::unordered_map<int, int> m_idToIndex;
	ShaderProgram* m_lit = nullptr;
	bool m_isWindows = true;

	const int MAX_LIGHTS = 4;

	glm::vec3 m_ambientColor = glm::vec3(1.0);
	float m_ambientStrength = 1.0;
};

} // namespace Uknitty
