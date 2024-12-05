#pragma once

#include "Interfaces.h"

namespace Uknitty
{
class Transform;
class Model;
class ICamera;
class ShaderProgram;
}

class SolidObject : public Uknitty::Render
{
public:
	SolidObject(Uknitty::ICamera* iCamera, Uknitty::Model* model, Uknitty::ShaderProgram* shaderProgram);

	Uknitty::ICamera* m_iCamera = nullptr;
	Uknitty::Model* m_model = nullptr;
	Uknitty::Transform* m_transform = nullptr;
	Uknitty::ShaderProgram* m_shaderProgram = nullptr;

	// Inherited via Render
	void Draw() override;
};

