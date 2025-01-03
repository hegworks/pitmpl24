#pragma once

#include "glm/glm.hpp"
#include "ModelObject.h"

namespace Uknitty
{

class PhysicsObject : public ModelObject
{
public:
	virtual void OnAwake() override;

	void OverridePosition(glm::vec3 pos);
	void SetColliderOffset(glm::vec3 offset);

protected:
	glm::vec3 m_modelDimensions = glm::vec3(1);
	glm::vec3 m_colliderOffset = glm::vec3(0);

	void SetModelPosToPhysicsPos();
};

} // namespace Uknitty
