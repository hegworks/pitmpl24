#pragma once

#include "glm/glm.hpp"

enum class LightType : int
{
	POINT_LIGHT = 0,
	DIR_LIGHT = 1,
	SPOT_LIGHT = 2,
};

struct LightData
{
	LightType lightType = LightType::POINT_LIGHT;

	glm::vec3 diffuseColor = glm::vec3(1);
	glm::vec3 specularColor = glm::vec3(1);
	float specularStrength = 1.0f;
	float shininess = 32.0f;

	bool isAutoUpdate = true; /// should Automatically update position and rotation by LightManager based on the GameObject's WorldTransform position and rotation
	glm::vec3 position = glm::vec3(0, 1, 0);
	glm::vec3 direction = glm::vec3(0, -1, 0);

	float attConst = 1.0f;
	float attLin = 0.09f;
	float attQuad = 0.032f;

	float cutOff = 14.0f;
	float outerCutOff = 18.0f;
};
