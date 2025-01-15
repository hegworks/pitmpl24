#pragma once

#include "GameObject.h"

namespace Uknitty
{

class ShaderProgram;

class Light : public GameObject
{
public:
	//~Light();

	void Initialize(ShaderProgram* shaderProgram);

private:

};

} // namespace Uknitty
