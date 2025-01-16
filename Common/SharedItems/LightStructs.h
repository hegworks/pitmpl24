#pragma once

#include "glm/glm.hpp"

struct LightData
{
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float specularStrength;
	float shininess;
};

struct DirLightData
{
	glm::vec3 direction;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float specularStrength;
	float shininess;
};
