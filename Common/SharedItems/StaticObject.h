#pragma once

#include "btBulletDynamicsCommon.h"
#include "glm/glm.hpp"
#include "PhysicsObject.h"

namespace Uknitty
{

class Model;
class ShaderProgram;

class StaticObject : public Uknitty::PhysicsObject
{
public:
	void InitializeWithBoxShape(Model* model, ShaderProgram* shaderProgram, glm::vec3 modelDimensions,
								int collisionGroup = btBroadphaseProxy::DefaultFilter,
								int collisionMask = btBroadphaseProxy::AllFilter);
};

} // namespace Uknitty
