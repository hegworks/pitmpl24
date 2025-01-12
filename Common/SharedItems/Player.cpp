#include "Player.h"

#include "AssetManager.h"
#include "btBulletDynamicsCommon.h"
#include "BTDebugDraw.h"
#include "CameraObject.h"
#include "CInput.h"
#include "CPhysics.h"
#include "CRender.h"
#include "CTransform.h"
#include "DynamicObject.h"
#include "DynamicObject.h"
#include "Enemy.h"
#include "Engine.h"
#include "GameObject.h"
#include "GameplayEvents.h"
#include "GameSettings.h"
#include "GameSharedDependencies.h"
#include "GeneralCamera.h"
#include "Model.h"
#include "PhysicsCollisionFilters.h"
#include "PhysicsManager.h"
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

	GameSharedDependencies::Set<Player>(this);

	Uknitty::Model* model = Uknitty::Engine::GetInstance()->GetAssetManager()->AutoGetModel("player", "../Common/Assets/Models/NakedSnake/NakedSnake.obj");
	Uknitty::ShaderProgram* shaderProgram = Uknitty::Engine::GetInstance()->GetAssetManager()->AutoGetShaderProgram(MAIN_SHADERPROGRAM);
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

	m_sceneManagerBlackboard = GameSharedDependencies::Get<SceneManagerBlackboard>();
	m_generalCamera = static_cast<Uknitty::GeneralCamera*>(Uknitty::Engine::GetInstance()->GetMainCamera());
	m_playerCInput = new PlayerCInput();
	GameObject::AddCInput(m_playerCInput);
	m_playerCInput->SetOnShootInput([this] { OnShootInput(); });

	m_gunPosObject = Uknitty::Engine::GetInstance()->CreateGameObject<GameObject>();
	m_gunPosObject->GetLocalTransform()->SetPosition(GUN_POS);
	m_gunPosObject->SetParent(this);

	m_hp = HP;
}

void Player::OnUpdate(float deltaTime)
{
	MoveIfInput(deltaTime);
	UpdateFeetPos();
	CheckCameraTypeToDisableDraw();
	SetModelPosToPhysicsPos();
}

void Player::OnDestroy()
{
	std::cout << "Destroying Player" << std::endl;
	delete m_userPointerData;

	Uknitty::Engine::GetInstance()->GetPhysicsManager()->RemoveContactTestRigidbody(GameObject::GetCPhysics()->GetRigidBody());
	Uknitty::DynamicObject::OnDestroy();
	GameSharedDependencies::Remove<Player>();
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

void Player::OnEnemyBulletHit()
{
	m_hp--;
	std::cout << "Player is Hit. hp: " << m_hp << std::endl;
	if(m_hp <= 0)
	{
		std::cout << "Player is dead and the game is lost";
		GameSharedDependencies::Get<GameplayEvents>()->OnPlayerDied();
	}
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
		else if(data->physicsType == Uknitty::PhysicsType::WIN)
		{
			//std::cout << "Player <-----> RoomChange" << static_cast<int>(data->roomChangeType) << std::endl;
			if(m_collidedWithRoomChangeCallback)
			{
				GameSharedDependencies::Get<GameplayEvents>()->OnPlayerWon();
			}
		}
	}
}

void Player::MoveIfInput(float deltaTime)
{
	glm::vec3 dir = glm::vec3(0);

	glm::vec3 forward = m_generalCamera->GetForward();
	if(m_generalCamera->GetCameraType() == Uknitty::GeneralCamera::FollowType::TOP_DOWN_FOLLOW)
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
	float targetAngle = glm::degrees(atan2(dir.x, dir.z));
	float currentAngle = Uknitty::GameObject::GetLocalTransform()->GetRotation()->y;

	// normalize angles to [0, 360] range
	targetAngle = fmod(targetAngle, 360.0f);
	if(targetAngle < 0)
	{
		targetAngle += 360.0f;
	}
	currentAngle = fmod(currentAngle, 360.0f);
	if(currentAngle < 0)
	{
		currentAngle += 360.0f;
	}

	// find the shortest angle
	float angleDiff = targetAngle - currentAngle;
	if(angleDiff > 180.0f) angleDiff -= 360.0f;
	if(angleDiff < -180.0f) angleDiff += 360.0f;

	// Interpolate to the new angle
	float newAngle = currentAngle + glm::mix(0.0f, angleDiff, 1.0);

	glm::vec3 rotation = glm::vec3(0, newAngle, 0);
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
	bool newDrawEnabledState = m_generalCamera->GetCameraType() != Uknitty::GeneralCamera::FollowType::FIRST_PERSON;

	if(m_lastDrawEnabledState == newDrawEnabledState) return;

	m_lastDrawEnabledState = newDrawEnabledState;

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

void Player::OnShootInput()
{
	CastGunRay();
}

void Player::CastGunRay()
{
	glm::vec3 fromVec = *m_generalCamera->GetWorldTransform()->GetPosition();
	glm::vec3 camForward = m_generalCamera->GetForward();
	glm::vec3 toVec = fromVec + camForward * 50.0f;

	const btVector3 from = Uknitty::CPhysics::GLMVec3ToBtVec3(fromVec);
	const btVector3 to = Uknitty::CPhysics::GLMVec3ToBtVec3(toVec);

#ifdef DEBUG_DRAW_PHYSICS
	btDynamicsWorld* dynamicsWorld = Uknitty::Engine::GetInstance()->GetDynamicsWorld();
	dynamicsWorld->getDebugDrawer()->drawLine(from, to, Uknitty::CPhysics::GetBtColor(Uknitty::CPhysics::Color::PINK));
#endif // DEBUG_DRAW_PHYSICS 

	btCollisionWorld::ClosestRayResultCallback  closestResults = btCollisionWorld::ClosestRayResultCallback(from, to);
	closestResults.m_collisionFilterGroup = COLL_GROUP_PLAYER;
	closestResults.m_collisionFilterMask = COLL_MASK_PLAYER;
	//closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;

	Uknitty::Engine::GetInstance()->GetDynamicsWorld()->rayTest(from, to, closestResults);

	if(closestResults.hasHit())
	{
		//btVector3 p = from.lerp(to, closestResults.m_closestHitFraction);

		// ignore hits behind the player
		glm::vec3 playerPos = *GameObject::GetWorldTransform()->GetPosition();
		glm::vec3 rayDirection = glm::normalize(toVec - fromVec);
		glm::vec3 hitPointToPlayer = glm::normalize(playerPos - Uknitty::CPhysics::BtVec3ToGLMVec3(closestResults.m_hitPointWorld));
		float dotProduct = glm::dot(rayDirection, hitPointToPlayer);
		if(dotProduct > 0.0f) return;

#ifdef DEBUG_DRAW_PHYSICS 
		//dynamicsWorld->getDebugDrawer()->drawSphere(p, 0.1f, Uknitty::CPhysics::GetBtColor(Uknitty::CPhysics::Color::CYAN));
		//dynamicsWorld->getDebugDrawer()->drawLine(p, p + closestResults.m_hitNormalWorld, Uknitty::CPhysics::GetBtColor(Uknitty::CPhysics::Color::CYAN));
#endif // DEBUG_DRAW_PHYSICS 

		if(closestResults.m_collisionObject->getUserPointer())
		{
			auto userPointerData = static_cast<Uknitty::UserPointerData*>(closestResults.m_collisionObject->getUserPointer());
			if(userPointerData->physicsType == Uknitty::PhysicsType::ENEMY)
			{
				Enemy* enemy = static_cast<Enemy*>(userPointerData->extraData);
				enemy->OnPlayerBulletHit();
				std::cout << "Player shoot enemy\n";
				//return true;
			}
		}
	}
}
