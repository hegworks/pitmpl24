#include "SolidObject.h"

#include "Common.h"
#include "Engine.h"
#include "GameObject.h"
#include "PhysicsCollisionFilters.h"
#include "ShaderProgram.h"
#include "SharedDependencies.h"
#include <iostream>

#if 0
SolidObject::SolidObject(Uknitty::Model* model, glm::vec3 modelDimensions, glm::vec3 position)
{
	Uknitty::GameObject* go = Uknitty::Engine::GetInstance()->CreateGameObject<SolidObject>();
}
#endif

void SolidObject::OnAwake()
{
	std::cout << "Awake SolidObject" << std::endl;
}
