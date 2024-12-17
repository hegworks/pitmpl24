#pragma once

#include "DynamicObject.h"

class GunBullet : public DynamicObject
{
public:
	GunBullet(Uknitty::ICamera* iCamera, Uknitty::ShaderProgram* shaderProgram, btDynamicsWorld* btDynamicsWorld);
	~GunBullet();

private:
	const glm::vec3 MODEL_DIMENSIONS = glm::vec3(0.25, 0.12, 0.12);
	const glm::vec3 INITIAL_POSITION = glm::vec3(0);
	const float MASS = 1.0f;
};
