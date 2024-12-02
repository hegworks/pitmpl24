#pragma once

#include "GameObject.h"

namespace Uknitty
{
class Transform;
class Model;
class ICamera;
class ShaderProgram;
}

class SolidObject : public GameObject
{
public:
	SolidObject(Uknitty::ICamera* iCamera, Uknitty::Model* model, Uknitty::ShaderProgram* shaderProgram);
	void Render();

	Uknitty::ICamera* m_iCamera = nullptr;
	Uknitty::Model* m_model = nullptr;
	Uknitty::Transform* m_transform = nullptr;
	Uknitty::ShaderProgram* m_shaderProgram = nullptr;
};

