#pragma once

#include "Interfaces.h"

namespace Uknitty
{
class Transform;
class Model;
class ICamera;
class ShaderProgram;
class Physics;
}

class SolidObject : public Uknitty::Render
{
public:
	SolidObject(Uknitty::ICamera* iCamera, Uknitty::Model* model, Uknitty::ShaderProgram* shaderProgram, glm::vec3 modelDimensions, glm::vec3 position);
	SolidObject(Uknitty::ICamera* iCamera, Uknitty::Model* model, Uknitty::ShaderProgram* shaderProgram, glm::vec3 modelDimensions, glm::vec3 position, glm::vec3 colliderPosition);
	~SolidObject();

	Uknitty::Transform* GetTransform() const { return m_transform; }
	Uknitty::Physics* GetPhysics() const { return m_physics; }

	// Inherited via Render
	void Draw() override;
	void Destroy() override;

private:
	Uknitty::ICamera* m_iCamera = nullptr;
	Uknitty::Model* m_model = nullptr;
	Uknitty::Transform* m_transform = nullptr;
	Uknitty::Physics* m_physics = nullptr;
	Uknitty::ShaderProgram* m_shaderProgram = nullptr;

	void SetDependencies(Uknitty::ICamera* iCamera, Uknitty::Model* model, Uknitty::ShaderProgram* shaderProgram);
};

