#include "SceneManager.h"

#include "AssetManager.h"
#include "btBulletDynamicsCommon.h"
#include "CollisionManager.h"
#include "CountdownTimer.h"
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
#include "LightObject.h"
#include "LightStructs.h"
#include "Model.h"
#include "ModelDataStorage.h"
#include "ModelDataStorage.h"
#include "PhysicsManager.h"
#include "Player.h"
#include "RNG.h"
#include "RoomChangeStorage.h"
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
	m_pointLightFlickeringTimer = new Uknitty::CountdownTimer(0.0f);

	GenerateRandomLights();
	CreateShaderProgram();
	CreatePlayer();
	CreateLights();
	LoadAllScenes();
	LoadScene(m_roomFinder->GetCurrentLevelId());
	ChangePointLightColorsInNewRoom();
}

SceneManager::~SceneManager()
{
	for(int i = 0; i < m_pointLights.size(); i++)
	{
		m_engine->DestroyGameObject(m_pointLights[i]);
	}
	m_engine->DestroyGameObject(m_centralSpotLight);
#ifdef WINDOWS_BUILD
	m_engine->DestroyGameObject(m_dirLight);
#endif
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

void SceneManager::Update(float deltaTime)
{
	UpdateCentralSpotLight(deltaTime);
	UpdatePointLightsFlickering(deltaTime);
	if(m_currentScene)
	{
		m_currentScene->Update(deltaTime);
	}
}

void SceneManager::AlarmCurrentScene()
{
	m_currentScene->EnterAlarmState();
	m_isCurrentSceneInAlarmState = true;
}

void SceneManager::UpdateCentralSpotLight(float deltaTime)
{
	float rotY = (*m_centralSpotLightModel->GetLocalTransform()->GetRotation()).y;
	if(m_isCurrentSceneInAlarmState)
	{
		rotY += CENTRAL_SPOTLIGHT_ROT_SPEED_ALARMED * deltaTime;
	}
	else
	{
		rotY += CENTRAL_SPOTLIGHT_ROT_SPEED * deltaTime;
	}
	m_centralSpotLightModel->GetLocalTransform()->SetRotation(glm::vec3(30, rotY, 0));

	LightData* lightData = m_centralSpotLight->GetLightData();
	if(m_isCurrentSceneInAlarmState)
	{
		lightData->diffuseColor = glm::vec3(10, 0, 0);
	}
	else
	{
		lightData->diffuseColor = glm::vec3(1 + 0.5 * glm::sin(rotY / 6), 1 + 0.5 * glm::sin(rotY / 12), 1 + 0.5 * glm::sin(rotY / 18));
	}
	lightData->specularColor = lightData->diffuseColor;
	m_centralSpotLight->SetLightData(lightData);
}

void SceneManager::UpdatePointLightsFlickering(float deltaTime)
{
	m_pointLightFlickeringTimer->Update(deltaTime);
	if(m_pointLightFlickeringTimer->IsFinished())
	{
		if(m_isTimerForTurningOff)
		{
			float randomOffTime = Uknitty::RNG::RandomFloat(0.1f, 0.8f);
			m_pointLightFlickeringTimer->SetNewDuration(randomOffTime);
			m_pointLightFlickeringTimer->Reset();
			m_isTimerForTurningOff = false;

			m_flickeringLightIndex = Uknitty::RNG::RandomInt(0, m_pointLights.size() - 1);
			LightData* lightData = m_pointLights[m_flickeringLightIndex]->GetLightData();
			m_colorBeforeFlickering = lightData->diffuseColor;
			lightData->diffuseColor = glm::vec3(0);
			lightData->specularColor = glm::vec3(0);
			m_pointLights[m_flickeringLightIndex]->SetLightData(lightData);
		}
		else
		{
			float randomOnTime = Uknitty::RNG::RandomFloat(0.1f, 2.0f);
			m_pointLightFlickeringTimer->SetNewDuration(randomOnTime);
			m_pointLightFlickeringTimer->Reset();
			m_isTimerForTurningOff = true;

			LightData* lightData = m_pointLights[m_flickeringLightIndex]->GetLightData();
			lightData->diffuseColor = m_colorBeforeFlickering;
			lightData->specularColor = m_colorBeforeFlickering;
			m_pointLights[m_flickeringLightIndex]->SetLightData(lightData);
		}
	}
}

void SceneManager::ChangePointLightColorsInNewRoom()
{
	m_isTimerForTurningOff = true;
	m_pointLightFlickeringTimer->Reset();

	for(int i = 0; i < m_pointLights.size(); i++)
	{
		LightData* lightData = m_pointLights[i]->GetLightData();
		lightData->diffuseColor = m_pointLightGenratedColors[(m_currentMapId * m_pointLights.size()) + i];
		lightData->specularColor = lightData->diffuseColor;
		m_pointLights[i]->SetLightData(lightData);
	}
}

void SceneManager::LoadScene(int mapId)
{
	m_currentScene = new Scene(mapId);
}

void SceneManager::LoadAllScenes()
{
	for(int i = 0; i < TOTAL_LEVEL_MAPS; i++)
	{
		Scene* scene = new Scene(i);
		delete scene;
	}
}

void SceneManager::GenerateRandomLights()
{
	m_pointLightPossibleColors = {
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 0, 1),
		glm::vec3(1, 1, 0),
		glm::vec3(0, 1, 1),
		glm::vec3(1, 0, 1),
	};

	for(int i = 0; i < m_pointLightGenratedColors.size(); i++)
	{
		m_pointLightGenratedColors[i] = m_pointLightPossibleColors[Uknitty::RNG::RandomInt(0, m_pointLightPossibleColors.size() - 1)];
	}
}

void SceneManager::ChangeScene()
{
	delete m_currentScene;
	m_isCurrentSceneInAlarmState = false;
	LoadScene(m_currentMapId);
	GameSharedDependencies::Get<GameplayEvents>()->OnNewSceneLoaded();
	ChangePointLightColorsInNewRoom();
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

	/*{
		ModelDataStorage::ModelData* inventoryGunModelData = m_modelDataStorage->GetModelData(ModelDataStorage::HAMBURGER);
		Uknitty::ModelObject* inventoryGun = m_engine->CreateGameObject<Uknitty::ModelObject>();
		inventoryGun->Initialize(m_engine->GetAssetManager()->AutoGetModel(ModelDataStorage::HAMBURGER, inventoryGunModelData->m_filePath), m_engine->GetAssetManager()->AutoGetShaderProgram(MAIN_SHADERPROGRAM));
		inventoryGun->GetLocalTransform()->SetScale(glm::vec3(1));
		inventoryGun->GetLocalTransform()->SetPosition(glm::vec3(0, 5, 0));
		m_engine->UseDefaultParent(inventoryGun);
	}*/
}

void SceneManager::CreateLights()
{
	m_engine->GetLightManager()->SetAmbientColor(glm::vec3(1.0));
#ifdef WINDOWS_BUILD
	m_engine->GetLightManager()->SetAmbientStrength(0.4f);
#endif
#ifdef Raspberry_BUILD
	m_engine->GetLightManager()->SetAmbientStrength(0.1f);
#endif

#ifdef WINDOWS_BUILD
	{ // Dir light
		Uknitty::LightObject* lightSource = m_engine->CreateGameObject<Uknitty::LightObject>();
		LightData* lightData = new LightData();
		lightData->lightType = LightType::DIR_LIGHT;
		lightData->isAutoUpdate = false;
		lightData->direction = glm::vec3(0.1, -1, 0.1);
		lightData->diffuseColor = glm::vec3(0.05);
		lightData->specularColor = glm::vec3(0.05);
		lightData->specularStrength = 0.2;
		lightData->shininess = 16;
		lightSource->SetLightData(lightData);
		m_dirLight = lightSource;
	}
#endif // WINDOWS_BUILD

	{ // Central Spot light
		ModelDataStorage::ModelData* lightModelData = m_modelDataStorage->GetModelData(ModelDataStorage::EMPTY);
		m_centralSpotLightModel = m_engine->CreateGameObject<Uknitty::ModelObject>();
		m_centralSpotLightModel->Initialize(m_engine->GetAssetManager()->AutoGetModel(ModelDataStorage::EMPTY, lightModelData->m_filePath), m_engine->GetAssetManager()->AutoGetShaderProgram(Uknitty::ShaderType::SINGLE_COLOR));
		m_centralSpotLightModel->GetLocalTransform()->SetScale(glm::vec3(0.25));
		m_centralSpotLightModel->GetLocalTransform()->SetPosition(glm::vec3(0, 3, 0));
		m_engine->UseDefaultParent(m_centralSpotLightModel);

		Uknitty::LightObject* lightSource = m_engine->CreateGameObject<Uknitty::LightObject>();
		LightData* lightData = new LightData();
		lightData->lightType = LightType::SPOT_LIGHT;
		lightData->isAutoUpdate = true;
		lightData->position = glm::vec3(0, 5, 0);
		lightData->direction = glm::vec3(0.1, -1, 0);
		lightData->diffuseColor = glm::vec3(1);
		lightData->specularColor = glm::vec3(1);
		lightData->cutOff = 20.0;
		lightData->outerCutOff = 22.0;
		lightData->specularStrength = 0.2;
		lightData->shininess = 16;
		lightSource->SetLightData(lightData);
		lightSource->SetParent(m_centralSpotLightModel);
		m_centralSpotLight = lightSource;
	}

	{ // Right Point light
		Uknitty::LightObject* lightSource = m_engine->CreateGameObject<Uknitty::LightObject>();
		lightSource->GetLocalTransform()->SetPosition(glm::vec3(8, 5.0, 0));

#if 0 // to view the light source
		ModelDataStorage::ModelData* lightModelData = m_modelDataStorage->GetModelData(ModelDataStorage::POINT_LIGHT);
		Uknitty::ModelObject* lightModelObject = m_engine->CreateGameObject<Uknitty::ModelObject>();
		lightModelObject->Initialize(m_engine->GetAssetManager()->AutoGetModel(ModelDataStorage::POINT_LIGHT, lightModelData->m_filePath), m_engine->GetAssetManager()->AutoGetShaderProgram(Uknitty::ShaderType::UNLIT));
		lightModelObject->GetLocalTransform()->SetScale(glm::vec3(0.25));
		lightModelObject->SetParent(lightSource);
#endif

		m_engine->UseDefaultParent(lightSource);
		LightData* lightData = new LightData();
		lightData->lightType = LightType::POINT_LIGHT;
		lightData->position = glm::vec3(8, 5.0, 0);
		lightData->diffuseColor = glm::vec3(1, 0, 0);
		lightData->specularColor = glm::vec3(1, 0, 0);
		lightData->specularStrength = 1.0;
		lightData->shininess = 32;
		lightData->isAutoUpdate = false;
		lightSource->SetLightData(lightData);
		m_pointLights[0] = lightSource;
	}

	{ // Left Point light
		Uknitty::LightObject* lightSource = m_engine->CreateGameObject<Uknitty::LightObject>();
		lightSource->GetLocalTransform()->SetPosition(glm::vec3(-8, 5.0, 0));
		m_engine->UseDefaultParent(lightSource);

#if 0 // to view the light source
		ModelDataStorage::ModelData* lightModelData = m_modelDataStorage->GetModelData(ModelDataStorage::POINT_LIGHT);
		Uknitty::ModelObject* lightModelObject = m_engine->CreateGameObject<Uknitty::ModelObject>();
		lightModelObject->Initialize(m_engine->GetAssetManager()->AutoGetModel(ModelDataStorage::POINT_LIGHT, lightModelData->m_filePath), m_engine->GetAssetManager()->AutoGetShaderProgram(Uknitty::ShaderType::UNLIT));
		lightModelObject->GetLocalTransform()->SetScale(glm::vec3(0.25));
		lightModelObject->SetParent(lightSource);
#endif
		LightData* lightData = new LightData();
		lightData->lightType = LightType::POINT_LIGHT;
		lightData->position = glm::vec3(-8, 5.0, 0);
		lightData->diffuseColor = glm::vec3(0, 1, 0);
		lightData->specularColor = glm::vec3(0, 1, 0);
		lightData->specularStrength = 1.0;
		lightData->shininess = 32;
		lightData->isAutoUpdate = false;
		lightSource->SetLightData(lightData);
		m_pointLights[1] = lightSource;
	}
}

void SceneManager::CreateShaderProgram()
{
	m_engine->GetAssetManager()->AutoGetShaderProgram(Uknitty::ShaderType::UNLIT, "../Common/Assets/Shaders/UnlitVertex.glsl", "../Common/Assets/Shaders/UnlitFragment.glsl");
	Uknitty::ShaderProgram* singleColorShader = m_engine->GetAssetManager()->AutoGetShaderProgram(Uknitty::ShaderType::SINGLE_COLOR, "../Common/Assets/Shaders/SingleColorVertex.glsl", "../Common/Assets/Shaders/SingleColorFragment.glsl");
	singleColorShader->Use();
	singleColorShader->SetVec4("uColor", glm::vec4(1));
	singleColorShader->UnUse();
}
