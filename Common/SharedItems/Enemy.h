#pragma once

#include "EnemyState.h"
#include "glm/glm.hpp"
#include "Interfaces.h"
#include <vector>

namespace Uknitty
{
class Transform;
class Model;
class ICamera;
class ShaderProgram;
class Physics;
}

class btDynamicsWorld;
class btCollisionObject;

class Enemy : public Uknitty::FlowRender
{
public:
	Enemy(Uknitty::Model* model, Uknitty::ICamera* camera, Uknitty::ShaderProgram* shaderProgram, btDynamicsWorld* btDynamicsWorld, std::vector<glm::vec3> patrolPositions);
	~Enemy();

	// Inherited via FlowRender
	void Awake() override;
	void Start() override;
	void Update(float deltaTime) override;
	void LateUpdate(float deltaTime) override;
	void FixedUpdate() override;
	void Destroy() override;
	void Draw() override;

	Uknitty::Physics* GetPhysics() const { return m_physics; }
	Uknitty::Transform* GetTransform() const { return m_transform; }

private:
	Uknitty::ICamera* m_iCamera = nullptr;
	Uknitty::ShaderProgram* m_shaderProgram = nullptr;
	Uknitty::Physics* m_physics = nullptr;
	btDynamicsWorld* m_btDynamicsWorld = nullptr;
	Uknitty::Model* m_model = nullptr;
	Uknitty::Transform* m_transform = nullptr;

	const float SPEED_WALK = 1.5f;
	const float SPEED_ROTATION = 1.7f;
	const glm::vec3 MODEL_DIMENSIONS = glm::vec3(1, 2, 1);

	float m_moveSpeed = SPEED_WALK;
	float m_rotationSpeed = SPEED_ROTATION;
	std::vector<glm::vec3> m_patrolPositions;

	EnemyState m_enemyState = EnemyState::PATROL;

	void OnCollision(const btCollisionObject* other);
};
