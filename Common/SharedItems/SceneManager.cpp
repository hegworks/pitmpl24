#include "SceneManager.h"

#include "AssetManager.h"
#include "btBulletDynamicsCommon.h"
#include "CollisionManager.h"
#include "CPhysics.h"
#include "CTransform.h"
#include "Engine.h"
#include "GameSettings.h"
#include "GameSharedDependencies.h"
#include "GeneralCamera.h"
#include "InterfaceManager.h"
#include "Model.h"
#include "ModelDataStorage.h"
#include "PhysicsManager.h"
#include "Player.h"
#include "RoomFinder.h"
#include "Scene.h"
#include "SceneManagerBlackboard.h"
#include "ShaderProgram.h"
#include <BTDebugDraw.h>

SceneManager::SceneManager()
{
	GameSharedDependencies::Set<SceneManager>(this);

	m_engine = Uknitty::Engine::GetInstance();
	m_roomFinder = new RoomFinder();
	new SceneManagerBlackboard();
	m_modelDataStorage = GameSharedDependencies::Get<ModelDataStorage>();

	CreateShaderProgram();
	CreatePlayer();
	LoadScene(m_roomFinder->GetCurrentLevelId());
}

SceneManager::~SceneManager()
{
	m_engine->GetPhysicsManager()->UnregisterListener(m_player->GetCPhysics()->GetRigidBody());
	m_engine->GetPhysicsManager()->RemoveContactTestRigidbody(m_player->GetCPhysics()->GetRigidBody());
	m_engine->DestroyGameObject(m_player);
	delete GameSharedDependencies::Get<SceneManagerBlackboard>();
	GameSharedDependencies::Remove<SceneManager>();
	delete m_currentScene;
	delete m_roomFinder;
}

void SceneManager::OnPlayerCollidedWithRoomChange(RoomChangeType roomChangeType)
{
	std::cout << "Player collided with room change: " << static_cast<int>(roomChangeType) << std::endl;

	m_isNewSceneLoading = true;
	RoomChange* newRoomChange = m_roomFinder->FindNextRoom(roomChangeType);
	m_player->RoomChangedSetPosition(newRoomChange);
	int newMapId = newRoomChange->nextRoomId;
	ChangeScene(newMapId);
	m_isNewSceneLoading = false;
	m_roomFinder->SetCurrentLevelId(newMapId);
}

void SceneManager::LoadScene(int mapId)
{
	m_currentScene = new Scene(mapId);
}

void SceneManager::ChangeScene(int mapId)
{
	m_engine->GetPhysicsManager()->RemoveContactTestRigidbody(m_player->GetCPhysics()->GetRigidBody());
	m_engine->GetPhysicsManager()->Disable();

	delete m_currentScene;
	m_currentMapId = mapId;
	LoadScene(m_currentMapId);

	m_engine->GetPhysicsManager()->Enable();
	m_engine->GetPhysicsManager()->AddContactTestRigidbody(m_player->GetCPhysics()->GetRigidBody());
}

void SceneManager::CreatePlayer()
{
	Uknitty::GeneralCamera* mainCamera = static_cast<Uknitty::GeneralCamera*>(m_engine->GetMainCamera());

	m_player = m_engine->CreateGameObject<Player>();
	m_engine->UseDefaultParent(m_player);
	m_engine->GetMainCamera()->SetFollowTransform(m_player->GetWorldTransform());
	mainCamera->SetMode(Uknitty::GeneralCamera::Mode::FOLLOW);
	m_engine->GetPhysicsManager()->RegisterListener(m_player->GetCPhysics()->GetRigidBody(), m_player->GetCPhysics());
	m_engine->GetPhysicsManager()->AddContactTestRigidbody(m_player->GetCPhysics()->GetRigidBody());
	m_player->SetCollidedWithRoomChangeCallback([this](RoomChangeType roomChangeType) { OnPlayerCollidedWithRoomChange(roomChangeType); });

	ModelDataStorage::ModelData* camraReticleModelData = m_modelDataStorage->GetModelData(ModelDataStorage::RETICLE);
	Uknitty::ModelObject* cameraReticle = m_engine->CreateGameObject<Uknitty::ModelObject>();
	cameraReticle->Initialize(m_engine->GetAssetManager()->AutoGetModel(ModelDataStorage::RETICLE, camraReticleModelData->m_filePath), m_engine->GetAssetManager()->AutoGetShaderProgram(MAIN_SHADERPROGRAM));
	cameraReticle->GetLocalTransform()->SetScale(glm::vec3(0.001f));
	cameraReticle->GetLocalTransform()->SetPosition(glm::vec3(0, 0, -0.1));
	cameraReticle->SetParent(m_engine->GetMainCamera());
}

void SceneManager::CreateShaderProgram()
{
	m_engine->GetAssetManager()->AutoGetShaderProgram(MAIN_SHADERPROGRAM, "../Common/Assets/Shaders/Vertex.glsl", "../Common/Assets/Shaders/Fragment.glsl");
}
