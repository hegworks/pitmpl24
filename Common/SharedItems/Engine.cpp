#include "Engine.h"

#include "AssetManager.h"
#include "AudioPlayer.h"
#include "btBulletDynamicsCommon.h"
#include "CInput.h"
#include "CRender.h"
#include "CTransform.h"
#include "GameObject.h"
#include "GeneralCamera.h"
#include "IInput.h"
#include "IInputKey.h"
#include "LightManager.h"
#include "PhysicsManager.h"
#include "ShaderProgram.h"
#include "UknittySettings.h"
#include <stdexcept>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace Uknitty
{

Engine::Engine()
{
	m_physicsManager = new Uknitty::PhysicsManager();
	m_assetManager = new Uknitty::AssetManager();
	m_lightManager = new Uknitty::LightManager(m_assetManager);
	m_audioPlayer = new Uknitty::AudioPlayer();
	m_root = CreateGameObject<GameObject>();
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

			if(m_iMouse->IsCapturingMouseInput())
			{
				cinput->ProcessMousePosition(m_iMouse->GetPosition().x, m_iMouse->GetPosition().y);
			}
		}

		gameObject->Update(deltaTime);
	}

	m_root->UpdateWorldTransform(glm::identity<glm::mat4>());

	// call main_camera's LateUpdate first.
	m_mainCamera->LateUpdate(deltaTime);
	for(auto& [id, gameObject] : m_gameObjects)
	{
		if(gameObject->GetID() != m_mainCamera->GetID())
		{
			gameObject->LateUpdate(deltaTime);
		}
	}

	m_root->UpdateWorldTransform(glm::identity<glm::mat4>());

	m_lightManager->Update(deltaTime);

	glm::mat4 cameraVP = m_mainCamera->GetProjection() * m_mainCamera->GetView();
	m_root->Draw(cameraVP);

	m_physicsManager->Draw(cameraVP);

#if 0
	glm::vec3 cameraLocalPos = *m_mainCamera->GetLocalTransform()->GetPosition();
	glm::vec3 cameraWorldPos = *m_mainCamera->GetWorldTransform()->GetPosition();
	std::cout << "cameraLocalPos: " << cameraLocalPos.x << ", " << cameraLocalPos.y << ", " << cameraLocalPos.z << std::endl;
	std::cout << "cameraWorldPos: " << cameraWorldPos.x << ", " << cameraWorldPos.y << ", " << cameraWorldPos.z << std::endl;
#endif
}

void Engine::Destroy()
{
	for(auto& [id, gameObject] : m_gameObjects)
	{
		delete gameObject;
	}
	m_gameObjects.clear();

	delete m_physicsManager;
	delete m_assetManager;
	delete m_lightManager;
}

void Engine::DestroyGameObject(GameObject* gameObject)
{
	DestroyGameObject(gameObject->GetID());
}

void Engine::DestroyGameObject(GameObject::ID id)
{
	if(m_gameObjects.find(id) != m_gameObjects.end())
	{
		m_gameObjects[id]->OnDestroy();
		m_gameObjects.erase(id);
	}
}

void Engine::RemoveGameObject(GameObject* gameObject)
{
	if(m_gameObjects.find(gameObject->GetID()) != m_gameObjects.end())
	{
		m_gameObjects.erase(gameObject->GetID());
	}
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
	m_mainCamera = CreateGameObject<Uknitty::GeneralCamera>();
	m_mainCamera->SetParent(m_root);
}

void Engine::UseDefaultParent(GameObject* child)
{
	if(!m_root)
	{
		throw std::runtime_error("Default parent not set");
	}
	child->SetParent(m_root);
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

void Engine::KeyUpAll()
{
	for(auto& [id, gameObject] : m_gameObjects)
	{
		if(gameObject->HasCInput())
		{
			gameObject->GetCInput()->OnKeyUpAll();
		}
	}
}

void Engine::MouseButtonDown(MouseButton mouseButton)
{
	for(auto& [id, gameObject] : m_gameObjects)
	{
		if(gameObject->HasCInput())
		{
			gameObject->GetCInput()->OnMouseButtonDown(mouseButton);
		}
	}
}

void Engine::MouseButtonUp(MouseButton mouseButton)
{
	for(auto& [id, gameObject] : m_gameObjects)
	{
		if(gameObject->HasCInput())
		{
			gameObject->GetCInput()->OnMouseButtonUp(mouseButton);
		}
	}
}

btDynamicsWorld* Engine::GetDynamicsWorld()
{
	return m_physicsManager->GetDynamicsWorld();
}

} // namespace Uknitty
