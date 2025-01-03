#pragma once

#include "btBulletDynamicsCommon.h"
#include "glm/glm.hpp"
#include "PhysicsObject.h"

namespace Uknitty
{

class CTransform;
class Model;
class CameraObject;
class ShaderProgram;
class Physics;

class DynamicObject : public PhysicsObject
{
public:
	void InitializeWithBoxShape(Model* model, ShaderProgram* shaderProgram, glm::vec3 modelDimensions, float mass,
								int collisionGroup = btBroadphaseProxy::DefaultFilter,
								int collisionMask = btBroadphaseProxy::AllFilter);

	void InitializeWithCapsuleShape(Model* model, ShaderProgram* shaderProgram, float radius, float height, float mass,
									int collisionGroup = btBroadphaseProxy::DefaultFilter,
									int collisionMask = btBroadphaseProxy::AllFilter);

	virtual void OnLateUpdate(float deltaTime) override;

	void MoveInDirection(glm::vec3 direction, float speed);
};

} // namespace Uknitty
