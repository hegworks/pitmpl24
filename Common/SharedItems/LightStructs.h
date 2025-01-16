#pragma once

#include "glm/glm.hpp"

struct LightData
{
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float specularStrength;
	float shininess;
	float attConst = 1.0f;
	float attLin = 0.09f;
	float attQuad = 0.032f;
};

struct DirLightData
{
	glm::vec3 direction;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float specularStrength;
	float shininess;
};
