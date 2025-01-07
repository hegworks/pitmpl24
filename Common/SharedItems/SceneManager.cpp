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
	m_engine = Uknitty::Engine::GetInstance();
	m_roomFinder = new RoomFinder();
	m_modelDataStorage = new ModelDataStorage();

	GameSharedDependencies::SetModelDataStorage(m_modelDataStorage);

	CreateShaderProgram();
	CreatePlayer();
	LoadScene(m_roomFinder->GetCurrentLevelId());
}

SceneManager::~SceneManager()
{
	m_engine->DestroyGameObject(m_player);
	delete m_currentScene;
	delete m_roomFinder;
	delete m_modelDataStorage;
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
	m_engine->GetPhysicsManager()->Disable();

	delete m_currentScene;
	m_currentMapId = mapId;
	LoadScene(m_currentMapId);

	m_engine->GetPhysicsManager()->Enable();
}

void SceneManager::CreatePlayer()
{
	m_player = m_engine->CreateGameObject<Player>();
	m_engine->UseDefaultParent(m_player);
	//m_engine->GetMainCamera()->SetFollowTransform(m_player->GetWorldTransform());
	m_engine->GetPhysicsManager()->RegisterListener(m_player->GetCPhysics()->GetRigidBody(), m_player->GetCPhysics());
	m_engine->GetPhysicsManager()->AddContactTestRigidbody(m_player->GetCPhysics()->GetRigidBody());
	m_player->SetCollidedWithRoomChangeCallback([this](RoomChangeType roomChangeType) { OnPlayerCollidedWithRoomChange(roomChangeType); });

	GameSharedDependencies::SetPlayer(m_player);

	ModelDataStorage::ModelData* pikminModelData = m_modelDataStorage->GetModelData(ModelDataStorage::PIKMIN);
	Uknitty::ModelObject* playerPikmin = m_engine->CreateGameObject<Uknitty::ModelObject>();
	playerPikmin->Initialize(m_engine->GetAssetManager()->AutoGetModel(ModelDataStorage::PIKMIN, pikminModelData->m_filePath), m_engine->GetAssetManager()->GetShaderProgram(MAIN_SHADERPROGRAM));
	playerPikmin->GetLocalTransform()->SetScale(glm::vec3(2));
	playerPikmin->GetLocalTransform()->SetPosition(glm::vec3(0, 0.5, 3));
	playerPikmin->SetParent(m_engine->GetMainCamera());
	//playerPikmin->SetParent(m_player);
}

void SceneManager::CreateShaderProgram()
{
	m_engine->GetAssetManager()->CreateShaderProgram(MAIN_SHADERPROGRAM, "../Common/Assets/Shaders/Vertex.glsl", "../Common/Assets/Shaders/Fragment.glsl");
}
