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
class AssetManager;

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
	~Engine() = default;
#pragma endregion Scott Meyers Thread-Safe Singleton

public:
	void Update(float deltaTime);
	void Destroy();

	template <typename T>
	T* CreateGameObject();

	void DestroyGameObject(GameObject* gameObject);
	void DestroyGameObject(GameObject::ID id);

	void InitializeInput(IMouse* iMouse, IKeyboard* iKeyboard);
	void ValidateBeforeFirstUpdate();

	void SetMainCamera(CameraObject* cameraObject);
	CameraObject* GetMainCamera();
	void CreateAndUseDefaultCamera();
	void SetDefaultParent(GameObject* defaultParent);
	void UseDefaultParent(GameObject* child);

	void KeyDown(Key key);
	void KeyUp(Key key);

	btDynamicsWorld* GetDynamicsWorld();
	PhysicsManager* GetPhysicsManager() { return m_physicsManager; }
	AssetManager* GetAssetManager() { return m_assetManager; }

private:
	GameObject::ID m_nextID = 0;

	std::unordered_map<GameObject::ID, GameObject*> m_gameObjects;
	Uknitty::PhysicsManager* m_physicsManager = nullptr;
	CameraObject* m_mainCamera = nullptr;
	Uknitty::AssetManager* m_assetManager = nullptr;
	GameObject* m_defaultParent = nullptr;

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

} // namespace Uknitty
