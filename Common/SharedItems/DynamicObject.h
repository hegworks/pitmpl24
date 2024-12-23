#pragma once

#include "glm/glm.hpp"

namespace Uknitty
{
class CTransform;
class Model;
class ICamera;
class ShaderProgram;
class Physics;
}

class btDynamicsWorld;

class DynamicObject
{
#if 0
public:
	DynamicObject() = default;
	~DynamicObject();

	void Construct(Uknitty::Model* model, glm::vec3 position, Uknitty::Physics* physics);
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
	Uknitty::CTransform* m_transform = nullptr;
	Uknitty::Physics* m_physics = nullptr;
	Uknitty::ShaderProgram* m_shaderProgram = nullptr;
	btDynamicsWorld* m_btDynamicsWorld = nullptr;

	glm::vec3 GetCurrentPhysicsPos();
	void SetModelPosToPhysicsPos();
#endif
};
