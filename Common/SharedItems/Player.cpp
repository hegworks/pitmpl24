#include "Player.h"

#include "GeneralCamera.h"
#include "ICamera.h"
#include "Interfaces.h"
#include "Model.h"
#include "Physics.h"
#include "PhysicsCollisionFilters.h"
#include "RoomChange.h"
#include "RoomChangePositionType.h"
#include "SceneManagerBlackboard.h"
#include "SharedDependencies.h"
#include "Transform.h"
#include <iostream>

Player::Player(Uknitty::Model* model)
{
	m_iCamera = SharedDependencies::GetCamera();
	m_shaderProgram = SharedDependencies::GetShaderProgram();
	m_btDynamicsWorld = SharedDependencies::GetDynamicsWorld();
	m_sceneManagerBlackboard = SceneManagerBlackboard::GetInstance();

	m_model = model;

	m_transform = new Uknitty::Transform();
	m_physics = new Uknitty::Physics();
	m_physics->InitialzeWithCapsuleShape(glm::vec3(0), MODEL_DIMENSIONS.x / 2.0, MODEL_DIMENSIONS.y / 2.0, 70.0f);
	m_physics->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
	m_physics->GetRigidBody()->setAngularFactor(btVector3(0, 0, 0)); // lock rotation
	m_physics->SetPosition(PLAYER_INITIAL_POS);
	auto userPointerData = new Uknitty::Physics::UserPointerData();
	userPointerData->physicsType = Uknitty::Physics::PhysicsType::PLAYER;
	userPointerData->name = "Player";
	m_physics->SetUserPointerData(userPointerData);
	m_physics->SetCollisionCallback([this](const btCollisionObject* other) { OnCollision(other); });
	m_btDynamicsWorld->addRigidBody(m_physics->GetRigidBody(), COLL_GROUP_PLAYER, COLL_MASK_PLAYER);
	std::cout << "Adding RigidBody: Player" << std::endl;
}

Player::~Player()
{
	m_btDynamicsWorld->removeRigidBody(m_physics->GetRigidBody());
	std::cout << "Removing RigidBody: Player" << " remaining: " << m_btDynamicsWorld->getNumCollisionObjects() << std::endl;
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
	m_sceneManagerBlackboard->SetPlayerFeetPos(rigidBodyPos);
	SetGunPos();
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
	m_model->Draw();
	m_shaderProgram->UnUse();
}

void Player::SetCollidedWithRoomChangeCallback(std::function<void(RoomChangeType roomChangeType)> callback)
{
	m_collidedWithRoomChangeCallback = callback;
}

void Player::RoomChangedSetPosition(RoomChange* roomChange)
{
	glm::vec3 currentPos = *m_transform->GetPosition();
	glm::vec2 newPos2D = glm::vec2(currentPos.x, currentPos.z);
	switch(roomChange->positionType)
	{
		case RoomChangePositionType::TOP:
		case RoomChangePositionType::BOTTOM:
			newPos2D.y = roomChange->newPlayerPos.y;
			break;
		case RoomChangePositionType::LEFT:
		case RoomChangePositionType::RIGHT:
			newPos2D.x = roomChange->newPlayerPos.x;
			break;
		case RoomChangePositionType::EXCEPTION:
			newPos2D = roomChange->newPlayerPos;
			break;
		default:
			throw std::runtime_error("Invalid room change position type");
	}
	glm::vec3 newPos3D = glm::vec3(newPos2D.x, currentPos.y, newPos2D.y);
	m_physics->SetPosition(newPos3D);
	glm::vec3 rigidBodyPos = newPos3D;
	newPos3D.y -= MODEL_DIMENSIONS.y / 2.0;
	m_transform->SetPosition(newPos3D);
	m_sceneManagerBlackboard->SetPlayerFeetPos(newPos3D);
	SetGunPos();
}

void Player::OnCollision(const btCollisionObject* other)
{
	if(other->getUserPointer())
	{
		auto data = static_cast<Uknitty::Physics::UserPointerData*>(other->getUserPointer());
		if(data->physicsType == Uknitty::Physics::PhysicsType::ROOM_CHANGE)
		{
			//std::cout << "Player <-----> RoomChange" << static_cast<int>(data->roomChangeType) << std::endl;
			if(m_collidedWithRoomChangeCallback)
			{
				m_collidedWithRoomChangeCallback(data->roomChangeType);
			}
		}
		else
		{
			//std::cout << "Player <-----> " << data->name << std::endl;
		}
	}
}

void Player::SetGunPos()
{
	glm::vec3 rigidBodyPos = Uknitty::Physics::BtVec3ToGLMVec3(m_physics->GetRigidBody()->getWorldTransform().getOrigin());
	glm::vec3 cameraForward = m_iCamera->GetForward();
	cameraForward.y = 0;
	m_sceneManagerBlackboard->SetPlayerGunPos(rigidBodyPos + glm::normalize(cameraForward) * 2.0f);
}
