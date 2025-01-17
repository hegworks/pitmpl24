#include "Scene.h"

#include "AssetManager.h"
#include "AStar.hpp"
#include "btBulletDynamicsCommon.h"
#include "CameraObject.h"
#include "CPhysics.h"
#include "CTransform.h"
#include "Enemy.h"
#include "Engine.h"
#include "GameSettings.h"
#include "GameSharedDependencies.h"
#include "GeneralCamera.h"
#include "GunBullet.h"
#include "InterfaceManager.h"
#include "Model.h"
#include "ModelDataStorage.h"
#include "ModelObject.h"
#include "PhysicsCollisionFilters.h"
#include "Player.h"
#include "SceneManagerBlackboard.h"
#include "ShaderProgram.h"
#include "ShaderType.h"
#include "StaticObject.h"
#include "StaticObject.h"
#include "UknittySettings.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <tmxparser.h>
#include <type_traits>
#include <vector>

Scene::Scene(int mapId)
{
	m_mapId = mapId;
	m_player = GameSharedDependencies::Get<Player>();
	m_modelDataStorage = GameSharedDependencies::Get<ModelDataStorage>();
	m_engine = Uknitty::Engine::GetInstance();
	m_shaderProgram = m_engine->GetAssetManager()->AutoGetShaderProgram(Uknitty::ShaderType::LIT);

	m_sceneManagerBlackboard = GameSharedDependencies::Get<SceneManagerBlackboard>();

	LoadMapDataFromFile(m_mapId);
	LoadObjectDataFromMap();
	CreateSolidObjectsFromData();
	CreateGround();
	CreatePathFinder();
	CreateEnemies();
}

Scene::~Scene()
{
	std::cout << "Destroying Scene: " << m_mapId << std::endl;

	for(auto& gameObject : m_createdGameObjects)
	{
		m_engine->DestroyGameObject(gameObject);
	}

	for(auto& wallData : m_wallDatas)
	{
		delete wallData;
	}
	m_wallDatas.clear();

	for(auto& roomChangeData : m_roomChangeDatas)
	{
		delete roomChangeData->userPointerData;
		delete roomChangeData;
	}
	m_roomChangeDatas.clear();

	m_enemiesPatrolPositions.clear();
	m_staticObjectsPositions.clear();
	m_createdGameObjects.clear();

	delete m_pathFinder;
}

void Scene::LoadMapDataFromFile(int mapId)
{
	std::string fullPath = MAPS_PATH + std::to_string(mapId) + MAPS_EXTENTION;
	std::cout << "Loading: " << fullPath << std::endl;
	tmxparser::TmxReturn error = tmxparser::parseFromFile(fullPath, &m_tmxMap, MAPS_PATH);
	if(error)
	{
		throw std::runtime_error("parseFromFile failed for file: " + fullPath);
	}
}

void Scene::LoadObjectDataFromMap()
{
	std::vector<tmxparser::TmxObjectGroup> objectGroups = m_tmxMap.objectGroupCollection;

	for(tmxparser::TmxObjectGroup& objectGroup : objectGroups)
	{
		if(objectGroup.name == CRATE_2_X_4_OBJECTGROUP)
		{
			for(tmxparser::TmxObject& object : objectGroup.objects)
			{
				m_staticObjectsPositions.emplace(ModelDataStorage::CRATE_2x4, MAP_CENTER - glm::vec2(object.x / TILE_SIZE, object.y / TILE_SIZE));
			}
		}
		if(objectGroup.name == CRATE_4_X_4_OBJECTGROUP)
		{
			for(tmxparser::TmxObject& object : objectGroup.objects)
			{
				m_staticObjectsPositions.emplace(ModelDataStorage::CRATE_4x4, MAP_CENTER - glm::vec2(object.x / TILE_SIZE, object.y / TILE_SIZE));
			}
		}
		if(objectGroup.name == TANK_OBJECTGROUP)
		{
			for(tmxparser::TmxObject& object : objectGroup.objects)
			{
				m_staticObjectsPositions.emplace(ModelDataStorage::TANK, MAP_CENTER - glm::vec2(object.x / TILE_SIZE, object.y / TILE_SIZE));
			}
		}
		if(objectGroup.name == FENCE_OBJECTGROUP)
		{
			for(tmxparser::TmxObject& object : objectGroup.objects)
			{
				m_staticObjectsPositions.emplace(ModelDataStorage::FENCE, MAP_CENTER - glm::vec2(object.x / TILE_SIZE, object.y / TILE_SIZE));
			}
		}
		if(objectGroup.name == WIN_OBJECTGROUP)
		{
			for(tmxparser::TmxObject& object : objectGroup.objects)
			{
				m_staticObjectsPositions.emplace(ModelDataStorage::WIN, MAP_CENTER - glm::vec2(object.x / TILE_SIZE, object.y / TILE_SIZE));
			}
		}
		if(objectGroup.name == WALL_OBJECTGROUP)
		{
			for(tmxparser::TmxObject& object : objectGroup.objects)
			{
				WallData* wallData = new WallData();
				wallData->position =
					MAP_CENTER
					- glm::vec2(
						(object.x + (object.width / 2.0f)) / TILE_SIZE,
						(object.y + (object.height / 2.0f)) / TILE_SIZE);
				wallData->size = glm::ivec2(object.width / TILE_SIZE, object.height / TILE_SIZE);
				wallData->wallType = static_cast<WallType>(std::stoi(object.propertyMap["vhu"]));
				m_wallDatas.push_back(wallData);
			}
		}
		if(objectGroup.name == RC_OBJECTGROUP)
		{
			for(tmxparser::TmxObject& object : objectGroup.objects)
			{
				RoomChangeData* roomChangeData = new RoomChangeData();
				roomChangeData->position =
					MAP_CENTER
					- glm::vec2(
						(object.x + (object.width / 2.0f)) / TILE_SIZE,
						(object.y + (object.height / 2.0f)) / TILE_SIZE);
				roomChangeData->size = glm::ivec2(object.width / TILE_SIZE, object.height / TILE_SIZE);
				roomChangeData->roomChangeType = static_cast<RoomChangeType>(std::stoi(object.propertyMap["RoomChangeType"]));
				m_roomChangeDatas.push_back(roomChangeData);
			}
		}
		if(objectGroup.name == ENEMY_PATROL_OBJECTGROUP)
		{
			for(tmxparser::TmxObject& object : objectGroup.objects)
			{
				int enemyIndex = std::stoi(object.type);
				int enemyPositionIndex = std::stoi(object.name);
				glm::vec2 enemyPosition2D = MAP_CENTER - glm::vec2(object.x / TILE_SIZE, object.y / TILE_SIZE);
				glm::vec3 enemyPosition3D = glm::vec3(enemyPosition2D.x, 0, enemyPosition2D.y);
				m_enemiesPatrolPositions[enemyIndex][enemyPositionIndex] = enemyPosition3D;
			}
		}
	}
}

void Scene::CreateSolidObjectsFromData()
{
	Uknitty::AssetManager* assetManager = m_engine->GetAssetManager();
	for(auto& [key, position] : m_staticObjectsPositions)
	{
		ModelDataStorage::ModelData* modelData = m_modelDataStorage->GetModelData(key);
		Uknitty::StaticObject* staticObject = m_engine->CreateGameObject<Uknitty::StaticObject>();
		m_createdGameObjects.push_back(staticObject);
		staticObject->InitializeWithBoxShape(assetManager->AutoGetModel(key, modelData->m_filePath), m_shaderProgram, modelData->m_dimensions, COLL_GROUP_OBSTACLE, COLL_MASK_OBSTACLE);
		staticObject->OverridePosition(glm::vec3(position.x, 0, position.y));
		m_engine->UseDefaultParent(staticObject);

		if(key == ModelDataStorage::WIN)
		{
			auto userPointerData = new Uknitty::UserPointerData();
			userPointerData->physicsType = Uknitty::PhysicsType::WIN;
			userPointerData->name = "win";
			staticObject->GetCPhysics()->SetUserPointerData(userPointerData);
		}
	}

#pragma region Pikmin
	{ // this is just a joke :D (but it also marks the center of the world)
		ModelDataStorage::ModelData* modelData = m_modelDataStorage->GetModelData(ModelDataStorage::PIKMIN);
		Uknitty::ModelObject* modelObject = m_engine->CreateGameObject<Uknitty::ModelObject>();
		m_createdGameObjects.push_back(modelObject);
		modelObject->Initialize(assetManager->AutoGetModel(ModelDataStorage::PIKMIN, modelData->m_filePath), m_shaderProgram);
		modelObject->GetLocalTransform()->SetScale(glm::vec3(2));
		modelObject->GetLocalTransform()->SetPosition(glm::vec3(0, 2, 0));
		m_engine->UseDefaultParent(modelObject);
	}
#pragma endregion Pikmin

#pragma region Wall
	for(auto& wallData : m_wallDatas)
	{
		switch(wallData->wallType)
		{
			case WallType::VERTICAL:
			{
				glm::vec3 wallVerticalScale = glm::vec3(wallData->size.x, 1, wallData->size.y);
				std::string keyPostFix = "_" + std::to_string(wallData->size.x) + "x" + std::to_string(wallData->size.y);

				Uknitty::Model* wallModel = nullptr;
				if(wallData->size.x == 1)
				{
					ModelDataStorage::ModelData* modelData = m_modelDataStorage->GetModelData(ModelDataStorage::WALL_1x4x1);
					std::string key = ModelDataStorage::WALL_1x4x1 + keyPostFix;
					wallModel = assetManager->AutoGetModel(key, modelData->m_filePath, glm::vec2(wallVerticalScale.z, 1));
				}
				else if(wallData->size.x == 2)
				{
					ModelDataStorage::ModelData* modelData = m_modelDataStorage->GetModelData(ModelDataStorage::WALL_2x4x1);
					std::string key = ModelDataStorage::WALL_1x4x1 + keyPostFix;
					wallModel = assetManager->AutoGetModel(key, modelData->m_filePath, glm::vec2(wallVerticalScale.z, 1));
				}
				else
				{
					throw std::runtime_error("vertical wall width bigger than 2 is not supported yet");
				}

				{
					glm::vec3 modelDimensions = glm::vec3(wallVerticalScale.x, 4, wallVerticalScale.z);
					Uknitty::StaticObject* staticObject = m_engine->CreateGameObject<Uknitty::StaticObject>();
					m_createdGameObjects.push_back(staticObject);
					staticObject->InitializeWithBoxShape(wallModel, m_shaderProgram, modelDimensions, COLL_GROUP_OBSTACLE, COLL_MASK_OBSTACLE);
					staticObject->GetLocalTransform()->SetScale(glm::vec3(1, 1, wallVerticalScale.z)); // x scale is built in the loaded wallModel
					staticObject->OverridePosition(glm::vec3(wallData->position.x, 0, wallData->position.y));
					m_engine->UseDefaultParent(staticObject);
				}

			}

			break;
			case WallType::HORIZONTAL:
			{

				glm::vec3 wallHorizontalScale = glm::vec3(wallData->size.x, 1, wallData->size.y);
				std::string keyPostFix = "_" + std::to_string(wallData->size.x) + "x" + std::to_string(wallData->size.y);

				Uknitty::Model* wallModel = nullptr;
				if(wallData->size.y == 1)
				{
					ModelDataStorage::ModelData* modelData = m_modelDataStorage->GetModelData(ModelDataStorage::WALL_1x4x1);
					std::string key = ModelDataStorage::WALL_1x4x1 + keyPostFix;
					wallModel = assetManager->AutoGetModel(key, modelData->m_filePath, glm::vec2(wallHorizontalScale.x, 1));
				}
				else if(wallData->size.y == 2)
				{
					ModelDataStorage::ModelData* modelData = m_modelDataStorage->GetModelData(ModelDataStorage::WALL_1x4x2);
					std::string key = ModelDataStorage::WALL_1x4x2 + keyPostFix;
					wallModel = assetManager->AutoGetModel(key, modelData->m_filePath, glm::vec2(wallHorizontalScale.x, 1));
				}
				else
				{
					throw std::runtime_error("horizontal wall width bigger than 2 is not supported yet");
				}

				{
					glm::vec3 modelDimensions = glm::vec3(wallHorizontalScale.x, 4, wallHorizontalScale.z);
					Uknitty::StaticObject* staticObject = m_engine->CreateGameObject<Uknitty::StaticObject>();
					m_createdGameObjects.push_back(staticObject);
					staticObject->InitializeWithBoxShape(wallModel, m_shaderProgram, modelDimensions, COLL_GROUP_OBSTACLE, COLL_MASK_OBSTACLE);
					staticObject->GetLocalTransform()->SetScale(glm::vec3(wallHorizontalScale.x, 1, 1)); // z scale is built in the loaded wallModel
					staticObject->OverridePosition(glm::vec3(wallData->position.x, 0, wallData->position.y));
					m_engine->UseDefaultParent(staticObject);
				}

			}

			break;

			case WallType::UNIFORM:
			{
				glm::vec3 wallUniformScale = glm::vec3(wallData->size.x, 1, wallData->size.y);
				std::string keyPostFix = "_" + std::to_string(wallData->size.x) + "x" + std::to_string(wallData->size.y);
				std::string key = ModelDataStorage::WALL_1x4x1 + keyPostFix;

				ModelDataStorage::ModelData* modelData = m_modelDataStorage->GetModelData(ModelDataStorage::WALL_1x4x1);
				Uknitty::Model* wallModel = assetManager->AutoGetModel(key, modelData->m_filePath, glm::vec2(wallUniformScale.x, 1));

				{
					glm::vec3 modelDimensions = glm::vec3(wallUniformScale.x, 4, wallUniformScale.z);
					Uknitty::StaticObject* staticObject = m_engine->CreateGameObject<Uknitty::StaticObject>();
					m_createdGameObjects.push_back(staticObject);
					staticObject->InitializeWithBoxShape(wallModel, m_shaderProgram, modelDimensions, COLL_GROUP_OBSTACLE, COLL_MASK_OBSTACLE);
					staticObject->GetLocalTransform()->SetScale(wallUniformScale);
					staticObject->OverridePosition(glm::vec3(wallData->position.x, 0, wallData->position.y));
					m_engine->UseDefaultParent(staticObject);
				}

			}

			break;

			default:
				throw std::runtime_error("uknown WallType");
		}
	}
#pragma endregion Wall

#pragma region RoomChange
	{
		ModelDataStorage::ModelData* modelData = m_modelDataStorage->GetModelData(ModelDataStorage::EMPTY);
		Uknitty::Model* model = assetManager->AutoGetModel(ModelDataStorage::EMPTY, modelData->m_filePath);
		for(auto& data : m_roomChangeDatas)
		{
			glm::vec3 scale = glm::vec3(data->size.x, 5, data->size.y);
			glm::vec3 modelDimensions = glm::vec3(scale.x, 5, scale.z);

			Uknitty::StaticObject* staticObject = m_engine->CreateGameObject<Uknitty::StaticObject>();
			m_createdGameObjects.push_back(staticObject);
			staticObject->InitializeWithBoxShape(model, m_shaderProgram, modelDimensions, COLL_GROUP_ROOMCHANGE, COLL_MASK_ROOMCHANGE);
			staticObject->GetLocalTransform()->SetScale(scale);
			staticObject->OverridePosition(glm::vec3(data->position.x, 0, data->position.y));
			m_engine->UseDefaultParent(staticObject);

			auto userPointerData = new Uknitty::UserPointerData();
			userPointerData->physicsType = Uknitty::PhysicsType::ROOM_CHANGE;
			userPointerData->roomChangeType = data->roomChangeType;
			staticObject->GetCPhysics()->SetUserPointerData(userPointerData);
			data->userPointerData = userPointerData;
		}
	}
#pragma endregion RoomChange
}

void Scene::CreateGround()
{
	ModelDataStorage::ModelData* modelData = m_modelDataStorage->GetModelData(ModelDataStorage::GROUND);
	Uknitty::StaticObject* staticObject = m_engine->CreateGameObject<Uknitty::StaticObject>();
	m_createdGameObjects.push_back(staticObject);
	staticObject->InitializeWithBoxShape
	(
		m_engine->GetAssetManager()->AutoGetModel
		(
			ModelDataStorage::GROUND, modelData->m_filePath, glm::vec2(MAP_SCALE_Z, MAP_SCALE_X)
		), m_shaderProgram, modelData->m_dimensions, COLL_GROUP_OBSTACLE, COLL_MASK_OBSTACLE
	);
	staticObject->SetColliderOffset(glm::vec3(0, -modelData->m_dimensions.y, 0));
	staticObject->GetLocalTransform()->SetScale(glm::vec3(MAP_SCALE_X, 1, MAP_SCALE_Z));
	m_engine->UseDefaultParent(staticObject);
}

void Scene::CreatePathFinder()
{
	m_pathFinder = new AStar::Generator();
	m_pathFinder->setWorldSize({MAP_SCALE_X, MAP_SCALE_Z});
	m_pathFinder->setHeuristic(AStar::Heuristic::manhattan);
	m_pathFinder->setDiagonalMovement(false);

	std::vector<tmxparser::TmxLayer> layers = m_tmxMap.layerCollection;

#ifdef DEBUG_DRAW_ASTAR_COLLISIONS
	Uknitty::Model* model = m_engine->GetAssetManager()->AutoGetModel(ModelDataStorage::CUBE, m_modelDataStorage->GetModelData(ModelDataStorage::CUBE)->m_filePath);
	glm::vec3 modelDimensions = glm::vec3(1, 6, 1);
#endif // DEBUG_DRAW_ASTAR_COLLISIONS

	for(tmxparser::TmxLayer& layer : layers)
	{
		if(layer.name == ASTAR_LAYER_COLLECTION)
		{
			for(unsigned int y = 0; y < layer.height; y++)
			{
				for(unsigned int x = 0; x < layer.width; x++)
				{
					if(layer.tiles[x + y * layer.width].gid == ASTAR_UNWALKABLE_GID)
					{
						m_pathFinder->addCollision({static_cast<int>(x), static_cast<int>(y)});

#ifdef DEBUG_DRAW_ASTAR_COLLISIONS
						glm::vec2 pos = MAP_CENTER - glm::vec2(x + 0.5f, y + 0.5f); // 0.5 is half of the dimension of the model
						Uknitty::ModelObject* DEBUG_OBJECT = m_engine->CreateGameObject<Uknitty::ModelObject>();
						m_createdGameObjects.push_back(DEBUG_OBJECT);
						DEBUG_OBJECT->Initialize(model, m_shaderProgram);
						DEBUG_OBJECT->GetLocalTransform()->SetScale(modelDimensions);
						DEBUG_OBJECT->GetLocalTransform()->SetPosition(glm::vec3(pos.x, 0, pos.y));
						m_engine->UseDefaultParent(DEBUG_OBJECT);
#endif // DEBUG_DRAW_ASTAR_COLLISIONS
					}
				}
			}
		}
	}
}

void Scene::CreateEnemies()
{
	int debug_maxEnemiesToSpawn = DEBUG_MAX_ENEMIES_TO_SPAWN;
	for(auto& [enemyIndex, patrolPosMap] : m_enemiesPatrolPositions)
	{
		if(--debug_maxEnemiesToSpawn < 0)
		{
			break;
		}

		std::vector<glm::vec3> patrolPositionsVector;
		for(auto& [patrolPositionIndex, patrolPosition] : patrolPosMap)
		{
			patrolPositionsVector.push_back(patrolPosition);
		}
		Enemy* enemy = m_engine->CreateGameObject<Enemy>();
		m_createdGameObjects.push_back(enemy);
		enemy->Initialize(std::move(patrolPositionsVector), m_pathFinder);
		m_engine->UseDefaultParent(enemy);

		if(enemyIndex == 0)
		{
			Uknitty::GameObject* enemyCameraOffsetObject = m_engine->CreateGameObject<Uknitty::GameObject>();
			enemyCameraOffsetObject->GetLocalTransform()->SetPosition(glm::vec3(0, 2.1, -0.5));
			enemyCameraOffsetObject->SetParent(enemy);

			/*Uknitty::ModelObject* enemyPikmin = m_engine->CreateGameObject<Uknitty::ModelObject>();
			enemyPikmin->Initialize(m_engine->GetAssetManager()->AutoGetModel(ModelDataStorage::PIKMIN),
									m_engine->GetAssetManager()->GetShaderProgram(MAIN_SHADERPROGRAM));
			enemyPikmin->GetLocalTransform()->SetPosition(glm::vec3(0, 2, 0));
			enemyPikmin->SetParent(enemy);*/

			//m_engine->GetMainCamera()->SetParent(enemyCameraOffsetObject);
			//Uknitty::GeneralCamera* generalCam = static_cast<Uknitty::GeneralCamera*>(m_engine->GetMainCamera());
			//generalCam->SetMode(Uknitty::GeneralCamera::Mode::CHILD_OF_STH);
		}
	}
}
