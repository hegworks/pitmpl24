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

class SolidObject : public Uknitty::Render
{
public:
	SolidObject(Uknitty::ICamera* iCamera, Uknitty::Model* model, Uknitty::ShaderProgram* shaderProgram, btDynamicsWorld* btDynamicsWorld, glm::vec3 modelDimensions, glm::vec3 position);
	~SolidObject();

	Uknitty::Transform* GetTransform() const { return m_transform; }
	Uknitty::Physics* GetPhysics() const { return m_physics; }

	// Inherited via Render
	virtual void Draw() override;
	virtual void Destroy() override;

private:
	Uknitty::ICamera* m_iCamera = nullptr;
	Uknitty::Model* m_model = nullptr;
	Uknitty::Transform* m_transform = nullptr;
	Uknitty::Physics* m_physics = nullptr;
	Uknitty::ShaderProgram* m_shaderProgram = nullptr;
	btDynamicsWorld* m_btDynamicsWorld = nullptr;

	void SetDependencies(Uknitty::ICamera* iCamera, Uknitty::Model* model, Uknitty::ShaderProgram* shaderProgram, btDynamicsWorld* btDynamicsWorld);
};

