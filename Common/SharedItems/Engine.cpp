#include "Engine.h"

#include "GameObject.h"
#include "PhysicsManager.h"

namespace Uknitty
{

Engine::Engine()
{
	m_physicsManager = new Uknitty::PhysicsManager();
}

Engine::~Engine()
{
	delete m_physicsManager;
}

void Engine::Update(float deltaTime)
{
}

} // namespace Uknitty
