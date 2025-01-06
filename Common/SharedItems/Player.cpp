#include "Player.h"

#include "AssetManager.h"
#include "CameraObject.h"
#include "CInput.h"
#include "CPhysics.h"
#include "CRender.h"
#include "CTransform.h"
#include "DynamicObject.h"
#include "DynamicObject.h"
#include "Engine.h"
#include "GameObject.h"
#include "GameSettings.h"
#include "GeneralCamera.h"
#include "GeneralCamera.h"
#include "Model.h"
#include "PhysicsCollisionFilters.h"
#include "PlayerCInput.h"
#include "RoomChange.h"
#include "RoomChangePositionType.h"
#include "SceneManagerBlackboard.h"
#include "ShaderProgram.h"
#include "UknittySettings.h"
#include <iostream>

void Player::OnAwake()
{
	Uknitty::DynamicObject::OnAwake();

	Uknitty::Model* model = Uknitty::Engine::GetInstance()->GetAssetManager()->AutoGetModel("player", "../Common/Assets/Models/NakedSnake/NakedSnake.obj");
	Uknitty::ShaderProgram* shaderProgram = Uknitty::Engine::GetInstance()->GetAssetManager()->GetShaderProgram(MAIN_SHADERPROGRAM);
	Uknitty::DynamicObject::InitializeWithCapsuleShape(model, shaderProgram, MODEL_DIMENSIONS.x, MODEL_DIMENSIONS.y, MASS, COLL_GROUP_PLAYER, COLL_MASK_PLAYER);

	Uknitty::CPhysics* cphysics = GameObject::GetCPhysics();
	cphysics->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
	cphysics->GetRigidBody()->setAngularFactor(btVector3(0, 0, 0)); // lock rotation

	m_userPointerData = new Uknitty::UserPointerData();
	m_userPointerData->physicsType = Uknitty::PhysicsType::PLAYER;
	m_userPointerData->name = "Player";
	cphysics->SetUserPointerData(m_userPointerData);

	cphysics->SetCollisionCallback([this](const btCollisionObject* other) { OnCollision(other); });

	Uknitty::DynamicObject::OverridePosition(PLAYER_INITIAL_POS);
	Uknitty::DynamicObject::SetColliderOffset(glm::vec3(0, -MODEL_DIMENSIONS.y / 2.0, 0));

	m_sceneManagerBlackboard = SceneManagerBlackboard::GetInstance();
	m_generalCamera = static_cast<Uknitty::GeneralCamera*>(Uknitty::Engine::GetInstance()->GetMainCamera());
	m_playerCInput = new PlayerCInput();
	GameObject::AddCInput(m_playerCInput);
}

void Player::OnUpdate(float deltaTime)
{
	MoveIfInput(deltaTime);
	UpdateFeetPos();
	CheckCameraTypeToDisableDraw();
}

void Player::OnDestroy()
{
	std::cout << "Destroying Player" << std::endl;
	delete m_userPointerData;
	
	Uknitty::DynamicObject::OnDestroy();
}

void Player::SetCollidedWithRoomChangeCallback(std::function<void(RoomChangeType roomChangeType)> callback)
{
	m_collidedWithRoomChangeCallback = callback;
}

void Player::RoomChangedSetPosition(RoomChange* roomChange)
{
	Uknitty::CPhysics* cphysics = Uknitty::DynamicObject::GetCPhysics();
	glm::vec3 currentPos = cphysics->GetPosition();
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
	Uknitty::DynamicObject::OverridePosition(newPos3D);
}

void Player::OnCollision(const btCollisionObject* other)
{
	if(other->getUserPointer())
	{
		auto data = static_cast<Uknitty::UserPointerData*>(other->getUserPointer());
		if(data->physicsType == Uknitty::PhysicsType::ROOM_CHANGE)
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

void Player::MoveIfInput(float deltaTime)
{
	glm::vec3 dir = glm::vec3(0);

	glm::vec3 forward = m_generalCamera->GetForward();
	if(m_generalCamera->GetCameraType() == Uknitty::GeneralCamera::CameraType::TOP_DOWN_FOLLOW)
	{
		forward.x = forward.x > 0.0f ? 1.0f : 0.0f;
		forward.y = forward.y > 0.0f ? 1.0f : 0.0f;
		forward.z = forward.z > 0.0f ? 1.0f : 0.0f;
	}
	glm::vec3 right = m_generalCamera->GetRight();

	if(m_playerCInput->IsForwardInput())
	{
		dir += forward;
	}
	if(m_playerCInput->IsBackwardInput())
	{
		dir -= forward;
	}
	if(m_playerCInput->IsRightInput())
	{
		dir += right;
	}
	if(m_playerCInput->IsLeftInput())
	{
		dir -= right;
	}
	dir.y = 0;

	if(glm::length(dir) > 0)
	{
		dir = glm::normalize(dir);
		Uknitty::DynamicObject::MoveInDirection(dir, m_moveSpeed);
		RotateGradually(dir, deltaTime);
	}
	else
	{
		Uknitty::DynamicObject::MoveInDirection(glm::vec3(0), 0);
	}
}

void Player::RotateGradually(glm::vec3 dir, float deltaTime)
{
	// REF: these calculations for gradual rotation are from ChatGPT
	float targetAngle = atan2(dir.x, dir.z);
	float currentAngle = glm::radians(Uknitty::GameObject::GetLocalTransform()->GetRotation()->y);
	float angleDifference = targetAngle - currentAngle;

	if(angleDifference > glm::pi<float>()) angleDifference -= 2 * glm::pi<float>();
	if(angleDifference < -glm::pi<float>()) angleDifference += 2 * glm::pi<float>();

	float newAngle = currentAngle + glm::clamp(angleDifference, -m_rotationSpeed * deltaTime, m_rotationSpeed * deltaTime);
	glm::vec3 rotation = glm::vec3(0, glm::degrees(newAngle), 0);
	Uknitty::GameObject::GetLocalTransform()->SetRotation(rotation);
}

void Player::UpdateFeetPos()
{
	glm::vec3 feetPos = Uknitty::DynamicObject::GetCPhysics()->GetPosition();
	feetPos.y -= MODEL_DIMENSIONS.y / 2.0f;
	m_sceneManagerBlackboard->SetPlayerFeetPos(feetPos);
}

void Player::CheckCameraTypeToDisableDraw()
{
	bool newDrawEnabledState = m_generalCamera->GetCameraType() != Uknitty::GeneralCamera::CameraType::FIRST_PERSON;

	if(m_lastDrawEnabledStated == newDrawEnabledState) return;

	m_lastDrawEnabledStated = newDrawEnabledState;

	if(newDrawEnabledState)
	{
		Uknitty::GameObject::EnableDrawSelf();
		Uknitty::GameObject::EnableDrawChildren();
	}
	else
	{
		Uknitty::GameObject::DisableDrawSelf();
		Uknitty::GameObject::DisableDrawChildren();
	}
}
