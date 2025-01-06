#pragma once

#include "GameObject.h"

#include <glm/glm.hpp>

namespace Uknitty
{

class CTransform;

/// <summary>
/// NOTE: every implementation of this class, MUST call the UpdateLocalTransformMatrix function as soon as the viewProjection matrix updates.
/// </summary>
class CameraObject : public Uknitty::GameObject
{
public:
	virtual float GetFov() const = 0;
	virtual glm::mat4 GetView() = 0;
	virtual glm::mat4 GetProjection() = 0;
	virtual glm::vec3 GetForward() = 0;
	virtual glm::vec3 GetRight() = 0;

	// optional
	virtual void SetFollowTransform([[maybe_unused]] Uknitty::CTransform* followTransform) {};
};

} // namespace Uknitty
