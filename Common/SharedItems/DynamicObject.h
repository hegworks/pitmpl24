#pragma once

#include "glm/glm.hpp"
#include "Interfaces.h"

namespace Uknitty
{
class Transform;
class Model;
class ICamera;
class ShaderProgram;
class Physics;
}

class btDynamicsWorld;

class DynamicObject : public Uknitty::FlowRender
{
public:
	DynamicObject(Uknitty::ICamera* iCamera, Uknitty::Model* model, Uknitty::ShaderProgram* shaderProgram, btDynamicsWorld* btDynamicsWorld, glm::vec3 position, Uknitty::Physics* physics);
	~DynamicObject();

	void SetPosition(glm::vec3 pos);

	// Inherited via FlowRender
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void FixedUpdate() override;
	virtual void Destroy() override;
	virtual void Draw() override;

protected:
	Uknitty::ICamera* m_iCamera = nullptr;
	Uknitty::Model* m_model = nullptr;
	Uknitty::Transform* m_transform = nullptr;
	Uknitty::Physics* m_physics = nullptr;
	Uknitty::ShaderProgram* m_shaderProgram = nullptr;
	btDynamicsWorld* m_btDynamicsWorld = nullptr;

	glm::vec3 GetCurrentPhysicsPos();
	void SetDependencies(Uknitty::ICamera* iCamera, Uknitty::Model* model, Uknitty::ShaderProgram* shaderProgram, btDynamicsWorld* btDynamicsWorld, Uknitty::Physics* physics);
	void SetModelPosToPhysicsPos();

};
