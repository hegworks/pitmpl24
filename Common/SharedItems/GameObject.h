#pragma once

#include "glm/glm.hpp"
#include <vector>

namespace Uknitty
{

class CInput;
class CRender;
class CPhysics;
class CTransform;

class GameObject
{
public:
	GameObject();
	~GameObject();

	/// every frame, 1st function
	void Update(float deltaTime)
	{
		if(m_isEnabled)
		{
			OnUpdate(deltaTime);
		}
	};
	/// every frame, after Update()
	void LateUpdate(float deltaTime)
	{
		if(m_isEnabled)
		{
			OnLateUpdate(deltaTime);
		}
	};
	/// every physics step
	void FixedUpdate()
	{
		if(m_isEnabled)
		{
			OnFixedUpdate();
		}
	};

	/// enables all Update functions
	void Enable()
	{
		m_isEnabled = true;
		OnEnable();
	}
	/// disables all Update functions
	void Disable()
	{
		m_isEnabled = false;
		OnDisable();
	}

	void Draw(glm::mat4 parentsMVP);

	/// 1 time, 1st function after constructor
	virtual void OnAwake() {};
	/// 1 time, after Awake()
	virtual void OnStart() {};

	virtual void OnUpdate(float deltaTime) {};
	virtual void OnLateUpdate(float deltaTime) {};
	virtual void OnFixedUpdate() {};
	virtual void OnEnable() {};
	virtual void OnDisable() {};
	virtual void OnDestroy() {};

	CTransform* GetLocalTransform() const { return m_localTransform; }
	CTransform* GetWorldTransform() const { return m_worldTransform; }

	void AddInput();
	void AddRender();
	void AddPhysics();

	CRender* GetRender() const;
	CPhysics* GetPhysics() const;
	CInput* GetInput() const;

	bool HasRender() const { return m_render != nullptr; }

private:
	bool m_isEnabled = false;

#pragma region Essential (Created in constructor)
	GameObject* m_parent = nullptr;
	std::vector<GameObject*> m_children;
	CTransform* m_localTransform = nullptr;
	CTransform* m_worldTransform = nullptr;
#pragma endregion Essential

#pragma region Optional (Can be added after construction by client)
	CInput* m_input = nullptr;
	CRender* m_render = nullptr;
	CPhysics* m_physics = nullptr;
#pragma endregion Optional

};

} // namespace Uknitty
