#pragma once

#include "GameObject.h"

namespace Uknitty
{

class Model;
class ShaderProgram;

class ModelObject : public Uknitty::GameObject
{
public:
	virtual void OnAwake() override;

	void Initialize(Model* model, ShaderProgram* shaderProgram);
};

} // namespace Uknitty
