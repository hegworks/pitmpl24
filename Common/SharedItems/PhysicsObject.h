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

protected:
	glm::vec3 m_modelDimensions = glm::vec3(1);

	void SetModelPosToPhysicsPos();
};

} // namespace Uknitty
