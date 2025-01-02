#pragma once

#include "btBulletDynamicsCommon.h"
#include "glm/glm.hpp"
#include "ModelObject.h"

namespace Uknitty
{

class Model;
class ShaderProgram;

class StaticObject : public Uknitty::ModelObject
{
public:
	virtual void OnAwake() override;

	void InitializeWithBoxShape(Model* model, ShaderProgram* shaderProgram, glm::vec3 modelDimensions,
								int collisionGroup = btBroadphaseProxy::DefaultFilter,
								int collisionMask = btBroadphaseProxy::AllFilter);

	void OverridePosition(glm::vec3 pos);

private:
	glm::vec3 m_modelDimensions;
};

} // namespace Uknitty
