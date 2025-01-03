#include "Engine.h"

#include "AssetManager.h"
#include "btBulletDynamicsCommon.h"
#include "CInput.h"
#include "CRender.h"
#include "CTransform.h"
#include "GameObject.h"
#include "GeneralCamera.h"
#include "IInput.h"
#include "IInputKey.h"
#include "PhysicsManager.h"
#include "UknittySettings.h"
#include <stdexcept>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Uknitty
{

Engine::Engine()
{
	m_physicsManager = new Uknitty::PhysicsManager();
	m_assetManager = new Uknitty::AssetManager();
}

Engine::~Engine()
{
	delete m_physicsManager;
}

void Engine::Update(float deltaTime)
{
	m_physicsManager->Update(deltaTime);

	for(auto& [id, gameObject] : m_gameObjects)
	{
		if(gameObject->HasCInput())
		{
			CInput* cinput = gameObject->GetCInput();
			cinput->ProcessKeyboard(m_iKeyboard);
			cinput->ProcessMousePosition(m_iMouse->GetPosition().x, m_iMouse->GetPosition().y);
		}

		gameObject->Update(deltaTime);
	}

	for(auto& [id, gameObject] : m_gameObjects)
	{
		gameObject->LateUpdate(deltaTime);
	}

	m_mainCamera->Draw(glm::identity<glm::mat4>());
	m_physicsManager->Draw(*m_mainCamera->GetLocalTransform()->GetMatrix());
}

void Engine::InitializeInput(IMouse* iMouse, IKeyboard* iKeyboard)
{
	if(m_iMouse || m_iKeyboard) throw std::runtime_error("Input already initialized");
	m_iMouse = iMouse;
	m_iKeyboard = iKeyboard;
}

void Engine::ValidateBeforeFirstUpdate()
{
	if(!m_iMouse || !m_iKeyboard) throw std::runtime_error("Input not initialized");
	if(!m_mainCamera) throw std::runtime_error("Main camera not set");
}

void Engine::SetMainCamera(CameraObject* cameraObject)
{
	m_mainCamera = cameraObject;
}

CameraObject* Engine::GetMainCamera()
{
	return m_mainCamera;
}

void Engine::CreateAndUseDefaultCamera()
{
	if(m_mainCamera) throw std::runtime_error("Main camera already exists");
	SetMainCamera(CreateGameObject<Uknitty::GeneralCamera>());
}

void Engine::KeyDown(Key key)
{
	for(auto& [id, gameObject] : m_gameObjects)
	{
		if(gameObject->HasCInput())
		{
			gameObject->GetCInput()->OnKeyDown(key);
		}
	}
}

void Engine::KeyUp(Key key)
{
	for(auto& [id, gameObject] : m_gameObjects)
	{
		if(gameObject->HasCInput())
		{
			gameObject->GetCInput()->OnKeyUp(key);
		}
	}
}

btDynamicsWorld* Engine::GetDynamicsWorld()
{
	return m_physicsManager->GetDynamicsWorld();
}

} // namespace Uknitty
