#pragma once

#include "glm/glm.hpp"
#include <unordered_set>

namespace Uknitty
{

class CInput;
class CRender;
class CPhysics;
class CTransform;

namespace SkeletalAnimation
{
class Animator;
}

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
	virtual ~GameObject();

	/// every frame, 1st function
	void Update(float deltaTime);

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

	void EnableDrawSelf()
	{
		m_isDrawSelfEnabled = true;
	}

	void DisableDrawSelf()
	{
		m_isDrawSelfEnabled = false;
	}

	void Destroy()
	{
		OnDestroy();
	}

	void EnableDrawChildren();
	void DisableDrawChildren();

	void Draw(glm::mat4 cameraVP);
	void UpdateWorldTransform(glm::mat4 parentsWorldTransform);

	/// 1 time, 1st function after constructor
	virtual void OnAwake() {};
	/// 1 time, after Awake()
	virtual void OnStart() {};

	virtual void OnUpdate([[maybe_unused]] float deltaTime) {};
	virtual void OnLateUpdate([[maybe_unused]] float deltaTime) {};
	virtual void OnFixedUpdate() {};
	virtual void OnEnable() {};
	virtual void OnDisable() {};
	virtual void OnDestroy();

	GameObject* GetParent() const { return m_parent; }
	void SetParent(GameObject* parent);
	void AddChild(GameObject* child);
	void RemoveChild(GameObject* child);

	CTransform* GetLocalTransform() const { return m_localTransform; }
	CTransform* GetWorldTransform() const { return m_worldTransform; }

	CInput* AddCInput(CInput* cinput);
	CRender* AddCRender();
	CRender* AddCustomCRender(CRender* crender);
	CPhysics* AddCPhysics();
	SkeletalAnimation::Animator* AddCAnimator();

	CRender* GetCRender() const;
	CPhysics* GetCPhysics() const;
	CInput* GetCInput() const;
	SkeletalAnimation::Animator* GetCAnimator() const;

	bool HasCRender() const { return m_render != nullptr; }
	bool HasCPhysics() const { return m_physics != nullptr; }
	bool HasCInput() const { return m_input != nullptr; }
	bool HasCAnimator() const { return m_animator != nullptr; }

private:
	bool m_isEnabled = true;
	bool m_isDrawSelfEnabled = true;

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
	SkeletalAnimation::Animator* m_animator = nullptr;
#pragma endregion Optional

};

} // namespace Uknitty
