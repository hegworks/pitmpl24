#pragma once

#include "GameObject.h"

namespace Uknitty
{

class Model;
class ShaderProgram;

class ModelObject : public Uknitty::GameObject
{
public:
	void Initialize(Model* model, ShaderProgram* shaderProgram);
	virtual void OnAwake() override;
};

} // namespace Uknitty
