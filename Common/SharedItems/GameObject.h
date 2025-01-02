#pragma once

#include "glm/glm.hpp"
#include <unordered_set>

namespace Uknitty
{

class CInput;
class CRender;
class CPhysics;
class CTransform;

class GameObject
{
public:
	using ID = unsigned long long;

private:
	ID m_id = 0;

public:
	ID GetID() const { return m_id; }
	void SetID(ID id) { m_id = id; }

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

	void SetParent(GameObject* parent);
	void AddChild(GameObject* child);

	CTransform* GetLocalTransform() const { return m_localTransform; }
	CTransform* GetWorldTransform() const { return m_worldTransform; }

	CInput* AddCInput(CInput* cinput);
	CRender* AddCRender();
	CPhysics* AddCPhysics();

	CRender* GetCRender() const;
	CPhysics* GetCPhysics() const;
	CInput* GetCInput() const;

	bool HasCRender() const { return m_render != nullptr; }
	bool HasCPhysics() const { return m_physics != nullptr; }
	bool HasCInput() const { return m_input != nullptr; }

private:
	bool m_isEnabled = true;

#pragma region Essential (Created in constructor)
	GameObject* m_parent = nullptr;
	std::unordered_set<GameObject*> m_children;
	CTransform* m_localTransform = nullptr;
	CTransform* m_worldTransform = nullptr;
#pragma endregion Essential

#pragma region Optional (Can be added after construction by client using related functions)
	CInput* m_input = nullptr;
	CRender* m_render = nullptr;
	CPhysics* m_physics = nullptr;
#pragma endregion Optional

};

} // namespace Uknitty
