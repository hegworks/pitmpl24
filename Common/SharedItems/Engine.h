#pragma once

#include "GameObject.h"
#include <type_traits>
#include <vector>

namespace Uknitty
{

class GameObject;
class PhysicsManager;

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

private:
	std::vector <GameObject*> m_gameObjects;
	Uknitty::PhysicsManager* m_physicsManager = nullptr;
};

template<typename T>
inline T* Engine::CreateGameObject()
{
	static_assert(std::is_base_of<GameObject, T>::value, "T must inherit from GameObject");
	T* gameObject = new T();
	gameObject->OnAwake();
	gameObject->OnStart();
	m_gameObjects.push_back(gameObject);
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

} // namespace Uknitty
