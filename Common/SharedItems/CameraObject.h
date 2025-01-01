#pragma once

#include "GameObject.h"

#include <glm/glm.hpp>

namespace Uknitty
{

class CameraObject : public Uknitty::GameObject
{
public:
	virtual float GetFov() const = 0;
	virtual glm::mat4 GetView() = 0;
	virtual glm::mat4 GetProjection() = 0;
	virtual glm::vec3 GetForward() = 0;
	virtual glm::vec3 GetRight() = 0;
};

} // namespace Uknitty
