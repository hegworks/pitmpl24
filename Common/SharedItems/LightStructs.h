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
	LightType lightType;

	glm::vec3 diffuseColor = glm::vec3(1);
	glm::vec3 specularColor = glm::vec3(1);
	float specularStrength = 1.0f;
	float shininess = 32.0f;

	float attConst = 1.0f;
	float attLin = 0.09f;
	float attQuad = 0.032f;

	float cutOff = 14.0f;
	float outerCutOff = 18.0f;
};

//struct DirLightData
//{
//	glm::vec3 direction;
//	glm::vec3 diffuseColor;
//	glm::vec3 specularColor;
//	float specularStrength;
//	float shininess;
//};
