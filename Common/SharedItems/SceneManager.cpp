#include "SceneManager.h"

#include "AssetManager.h"
#include "btBulletDynamicsCommon.h"
#include "CollisionManager.h"
#include "CPhysics.h"
#include "CTransform.h"
#include "Engine.h"
#include "GameplayEvents.h"
#include "GameSettings.h"
#include "GameSharedDependencies.h"
#include "GeneralCamera.h"
#include "InterfaceManager.h"
#include "Light.h"
#include "LightManager.h"
#include "LightSource.h"
#include "LightStructs.h"
#include "Model.h"
#include "ModelDataStorage.h"
#include "ModelDataStorage.h"
#include "PhysicsManager.h"
#include "Player.h"
#include "RoomFinder.h"
#include "Scene.h"
#include "SceneManagerBlackboard.h"
#include "ShaderProgram.h"
#include "ShaderType.h"
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

	RoomChange* newRoomChange = m_roomFinder->FindNextRoom(roomChangeType);
	m_player->RoomChangedSetPosition(newRoomChange);
	int newMapId = newRoomChange->nextRoomId;
	m_currentMapId = newMapId;
	m_roomFinder->SetCurrentLevelId(newMapId);

	GameSharedDependencies::Get<GameplayEvents>()->OnPlayerCollidedWithRoomChange();
}

void SceneManager::LoadScene(int mapId)
{
	m_currentScene = new Scene(mapId);
}

void SceneManager::ChangeScene()
{
	delete m_currentScene;
	LoadScene(m_currentMapId);
	GameSharedDependencies::Get<GameplayEvents>()->OnNewSceneLoaded();
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
	cameraReticle->Initialize(m_engine->GetAssetManager()->AutoGetModel(ModelDataStorage::RETICLE, camraReticleModelData->m_filePath), m_engine->GetAssetManager()->AutoGetShaderProgram(Uknitty::ShaderType::DEFAULT));
	cameraReticle->GetLocalTransform()->SetScale(glm::vec3(0.001f));
	cameraReticle->GetLocalTransform()->SetPosition(glm::vec3(0, 0, -0.1));
	cameraReticle->SetParent(m_engine->GetMainCamera());

	m_engine->GetLightManager()->SetAmbientColor(glm::vec3(1.0));
	m_engine->GetLightManager()->SetAmbientStrength(0.1f);

	{
		Uknitty::LightSource* lightSource = m_engine->CreateGameObject<Uknitty::LightSource>();
		lightSource->GetLocalTransform()->SetPosition(glm::vec3(0, 2.0, 0));
		lightSource->SetParent(m_player);
		LightData* lightData = new LightData();
		lightData->lightType = LightType::SPOT_LIGHT;
		lightData->diffuseColor = glm::vec3(0, 1, 0);
		lightData->specularColor = glm::vec3(0, 1, 0);
		lightData->specularStrength = 0.5;
		lightData->shininess = 32;
		lightData->isStatic = false;
		lightSource->SetLightData(lightData);

		Uknitty::ModelObject* lightSourceModel = m_engine->CreateGameObject<Uknitty::ModelObject>();
		ModelDataStorage::ModelData* lightSourceModelData = GameSharedDependencies::Get<ModelDataStorage>()->GetModelData(ModelDataStorage::INVENTORY_GUN);
		lightSourceModel->Initialize(m_engine->GetAssetManager()->AutoGetModel(ModelDataStorage::INVENTORY_GUN, lightSourceModelData->m_filePath), m_engine->GetAssetManager()->AutoGetShaderProgram(Uknitty::ShaderType::UNLIT));
		lightSourceModel->GetLocalTransform()->SetScale(glm::vec3(0.2));
		lightSourceModel->SetParent(lightSource);
		m_engine->GetLightManager()->SetUnlitColor(glm::vec3(1));
	}

	{
		Uknitty::LightSource* lightSource = m_engine->CreateGameObject<Uknitty::LightSource>();
		lightSource->GetLocalTransform()->SetPosition(glm::vec3(0, 2.0, 0));
		LightData* lightData = new LightData();
		lightData->lightType = LightType::DIR_LIGHT;
		lightData->isStatic = true;
		lightData->direction = glm::vec3(0, -1, 0);
		lightData->diffuseColor = glm::vec3(1, 0, 0);
		lightData->specularColor = glm::vec3(1, 0, 0);
		lightData->specularStrength = 0.5;
		lightData->shininess = 32;
		lightSource->SetLightData(lightData);
	}

	{
		Uknitty::LightSource* lightSource = m_engine->CreateGameObject<Uknitty::LightSource>();
		lightSource->GetLocalTransform()->SetPosition(glm::vec3(0, 2.0, 0));
		lightSource->SetParent(m_player);
		LightData* lightData = new LightData();
		lightData->lightType = LightType::POINT_LIGHT;
		lightData->diffuseColor = glm::vec3(0, 0, 1);
		lightData->specularColor = glm::vec3(0, 0, 1);
		lightData->specularStrength = 1.0;
		lightData->shininess = 128;
		lightData->isStatic = false;
		lightSource->SetLightData(lightData);
	}

	m_engine->GetLightManager()->SetAmbientColor(glm::vec3(1.0));
	m_engine->GetLightManager()->SetAmbientStrength(0.1f);

	/*{
		ModelDataStorage::ModelData* inventoryGunModelData = m_modelDataStorage->GetModelData(ModelDataStorage::HAMBURGER);
		Uknitty::ModelObject* inventoryGun = m_engine->CreateGameObject<Uknitty::ModelObject>();
		inventoryGun->Initialize(m_engine->GetAssetManager()->AutoGetModel(ModelDataStorage::HAMBURGER, inventoryGunModelData->m_filePath), m_engine->GetAssetManager()->AutoGetShaderProgram(MAIN_SHADERPROGRAM));
		inventoryGun->GetLocalTransform()->SetScale(glm::vec3(1));
		inventoryGun->GetLocalTransform()->SetPosition(glm::vec3(0, 5, 0));
		m_engine->UseDefaultParent(inventoryGun);
	}*/
}

void SceneManager::CreateShaderProgram()
{
	m_engine->GetAssetManager()->AutoGetShaderProgram(Uknitty::ShaderType::DEFAULT, "../Common/Assets/Shaders/Vertex.glsl", "../Common/Assets/Shaders/Fragment.glsl");
	m_engine->GetAssetManager()->AutoGetShaderProgram(Uknitty::ShaderType::UNLIT, "../Common/Assets/Shaders/UnlitVertex.glsl", "../Common/Assets/Shaders/UnlitFragment.glsl");
	m_engine->GetAssetManager()->AutoGetShaderProgram(Uknitty::ShaderType::PHONG, "../Common/Assets/Shaders/PhongVertex.glsl", "../Common/Assets/Shaders/PhongFragment.glsl");
}
