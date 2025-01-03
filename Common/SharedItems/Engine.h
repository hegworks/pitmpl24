#pragma once

#include "btBulletDynamicsCommon.h"
#include "GameObject.h"
#include "IInputKey.h"
#include <type_traits>
#include <unordered_map>

class IMouse;
class IKeyboard;

namespace Uknitty
{

class GameObject;
class PhysicsManager;
class CameraObject;

class Engine
{
#pragma region Scott Meyers Thread-Safe Singleton
public:
	static Engine* GetInstance()
	{
		static Engine instance;
		return &instance;
	}

	Engine(Engine const&) = delete;
	void operator=(Engine const&) = delete;

private:
	Engine();
	~Engine();
#pragma endregion Scott Meyers Thread-Safe Singleton

public:
	void Update(float deltaTime);

	template <typename T>
	T* CreateGameObject();

	template <typename T>
	void DestroyGameObject(T* gameObject);
	void DestroyGameObject(GameObject::ID id);

	void InitializeInput(IMouse* iMouse, IKeyboard* iKeyboard);
	void ValidateBeforeFirstUpdate();

	void SetMainCamera(CameraObject* cameraObject);
	CameraObject* GetMainCamera();
	void CreateAndUseDefaultCamera();

	void KeyDown(Key key);
	void KeyUp(Key key);

	btDynamicsWorld* GetDynamicsWorld();
	PhysicsManager* GetPhysicsManager() { return m_physicsManager; }

private:
	GameObject::ID m_nextID = 0;

	std::unordered_map<GameObject::ID, GameObject*> m_gameObjects;
	Uknitty::PhysicsManager* m_physicsManager = nullptr;
	CameraObject* m_mainCamera = nullptr;

	IMouse* m_iMouse = nullptr;
	IKeyboard* m_iKeyboard = nullptr;
};

template<typename T>
inline T* Engine::CreateGameObject()
{
	static_assert(std::is_base_of<GameObject, T>::value, "T must inherit from GameObject");
	T* gameObject = new T();
	GameObject::ID newId = m_nextID++;
	gameObject->SetID(newId);
	gameObject->OnAwake();
	gameObject->OnStart();
	m_gameObjects[newId] = gameObject;
	return gameObject;
}

template<typename T>
inline void Engine::DestroyGameObject(T* gameObject)
{
	static_assert(std::is_base_of<GameObject, T>::value, "T must inherit from GameObject");
	auto it = std::find(m_gameObjects.begin(), m_gameObjects.end(), gameObject);
	if(it != m_gameObjects.end())
	{
		m_gameObjects.erase(it);
		gameObject->OnDestroy();
		delete gameObject;
	}
}

inline void Engine::DestroyGameObject(GameObject::ID id)
{
	auto it = m_gameObjects.find(id);
	if(it != m_gameObjects.end())
	{
		m_gameObjects.erase(it);
		it->second->OnDestroy();
		delete it->second;
	}
}

} // namespace Uknitty
