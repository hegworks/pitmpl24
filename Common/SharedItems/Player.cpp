#include "GeneralCamera.h"
#include "ICamera.h"
#include "Interfaces.h"
#include "Model.h"
#include "Physics.h"
#include "Player.h"
#include "Transform.h"
#include <iostream>

Player::Player(Uknitty::Model* model, Uknitty::ICamera* camera, Uknitty::ShaderProgram* shaderProgram, btDynamicsWorld* btDynamicsWorld)
{
	m_model = model;
	m_iCamera = camera;
	m_shaderProgram = shaderProgram;
	m_btDynamicsWorld = btDynamicsWorld;

	m_transform = new Uknitty::Transform();
	m_physics = new Uknitty::Physics();
	m_physics->InitialzeWithCapsuleShape(glm::vec3(0), MODEL_DIMENSIONS.x / 2.0, MODEL_DIMENSIONS.y / 2.0, 70.0f);
	m_physics->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
	m_physics->GetRigidBody()->setAngularFactor(btVector3(0, 0, 0)); // lock rotation
	auto userPointerData = new Uknitty::Physics::UserPointerData();
	userPointerData->physicsType = Uknitty::Physics::PhysicsType::PLAYER;
	userPointerData->name = "Player";
	m_physics->SetUserPointerData(userPointerData);
	m_physics->SetCollisionCallback([this](const btCollisionObject* other) { OnCollision(other); });
	m_btDynamicsWorld->DebugAddRigidBody(m_physics->GetRigidBody(), "Player");
}

Player::~Player()
{
	m_btDynamicsWorld->DebugRemoveRigidBody(m_physics->GetRigidBody(), "Player");
	delete m_physics;
	delete m_transform;
}

void Player::ProcessMousePosition(double xPos, double yPos)
{
}

void Player::KeyDown(Key key)
{
	if(key == FORWARD_KEY) m_isForwardKeyDown = true;
	if(key == BACKWARD_KEY) m_isBackwardKeyDown = true;
	if(key == LEFT_KEY) m_isLeftKeyDown = true;
	if(key == RIGHT_KEY) m_isRightKeyDown = true;
}

void Player::KeyUp(Key key)
{
	if(key == FORWARD_KEY) m_isForwardKeyDown = false;
	if(key == BACKWARD_KEY) m_isBackwardKeyDown = false;
	if(key == LEFT_KEY) m_isLeftKeyDown = false;
	if(key == RIGHT_KEY) m_isRightKeyDown = false;
}

void Player::ProcessKeyboard(IKeyboard* iKeyboard)
{
}

void Player::Awake()
{
}

void Player::Start()
{
}

void Player::Update(float deltaTime)
{
	glm::vec3 movement = glm::vec3(0);
	glm::vec3 forward = m_iCamera->GetForward();
	if(m_iCamera->GetCameraType() == Uknitty::ICamera::CameraType::TOP_DOWN_FOLLOW)
	{
		forward.x = forward.x > 0.0f ? 1.0f : 0.0f;
		forward.y = forward.y > 0.0f ? 1.0f : 0.0f;
		forward.z = forward.z > 0.0f ? 1.0f : 0.0f;
	}
	glm::vec3 right = m_iCamera->GetRight();
	if(m_isForwardKeyDown)
	{
		movement += forward;
	}
	if(m_isBackwardKeyDown)
	{
		movement -= forward;
	}
	if(m_isRightKeyDown)
	{
		movement += right;
	}
	if(m_isLeftKeyDown)
	{
		movement -= right;
	}
	movement.y = 0;
	if(glm::length(movement) > 0)
	{
		movement = glm::normalize(movement);
		m_physics->GetRigidBody()->setLinearVelocity(btVector3(movement.x * m_moveSpeed, 0, movement.z * m_moveSpeed));

		// REF: these calculations for gradual rotation are from ChatGPT
		float targetAngle = atan2(movement.x, movement.z);
		float currentAngle = glm::radians(m_transform->GetRotation()->y);
		float angleDifference = targetAngle - currentAngle;

		if(angleDifference > glm::pi<float>()) angleDifference -= 2 * glm::pi<float>();
		if(angleDifference < -glm::pi<float>()) angleDifference += 2 * glm::pi<float>();

		float newAngle = currentAngle + glm::clamp(angleDifference, -m_rotationSpeed * deltaTime, m_rotationSpeed * deltaTime);
		glm::vec3 rotation = glm::vec3(0, glm::degrees(newAngle), 0);
		m_transform->SetRotation(rotation);
	}
	else
	{
		btVector3 linearVelocity = m_physics->GetRigidBody()->getLinearVelocity();
		// we don't want to affect gravity
		m_physics->GetRigidBody()->setLinearVelocity(btVector3(0, linearVelocity.getY(), 0));
	}
	glm::vec3 rigidBodyPos = Uknitty::Physics::BtVec3ToGLMVec3(m_physics->GetRigidBody()->getWorldTransform().getOrigin());
	rigidBodyPos.y -= MODEL_DIMENSIONS.y / 2.0;
	m_transform->SetPosition(rigidBodyPos);
}

void Player::LateUpdate(float deltaTime)
{
}

void Player::FixedUpdate()
{
}

void Player::Destroy()
{
	std::cout << "Destroying Player" << std::endl;
	delete this;
}

void Player::Draw()
{
	if(m_iCamera->GetCameraType() == GeneralCamera::CameraType::FIRST_PERSON)
	{
		return;
	}
	m_shaderProgram->Use();
	m_shaderProgram->SetMat4("uView", m_iCamera->GetView());
	m_shaderProgram->SetMat4("uProjection", m_iCamera->GetProjection());
	m_shaderProgram->SetMat4("uModel", *m_transform->GetModelMatrix());
	glDisable(GL_BLEND);
	m_model->Draw(*m_shaderProgram);
	m_shaderProgram->UnUse();
}

void Player::OnCollision(const btCollisionObject* other)
{
	if(other->getUserPointer())
	{
		auto data = static_cast<Uknitty::Physics::UserPointerData*>(other->getUserPointer());
		if(data->physicsType == Uknitty::Physics::PhysicsType::ROOM_CHANGE)
		{
			std::cout << "Player <-----> RoomChange" << static_cast<int>(data->roomChangeType) << std::endl;
		}
		else
		{
			std::cout << "Player <-----> " << data->name << std::endl;
		}
	}
}
