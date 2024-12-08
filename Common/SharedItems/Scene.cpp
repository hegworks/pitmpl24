#include "Scene.h"

#include "ICamera.h"
#include "InterfaceManager.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "SolidObject.h"
#include "Transform.h"
#include <iostream>
#include <stdexcept>
#include <tmxparser.h>

Scene::Scene(int mapId, Uknitty::ICamera* camera, Uknitty::ShaderProgram* shaderProgram, Player* player)
{
	m_mapId = mapId;
	m_iCamera = camera;
	m_shaderProgram = shaderProgram;
	m_player = player;
}

void Scene::ProcessMousePosition(double xPos, double yPos)
{
	m_interfaceManager->ProcessMousePosition(xPos, yPos);
}

void Scene::ProcessKeyboard(IKeyboard* iKeyboard)
{
	m_interfaceManager->ProcessKeyboard(iKeyboard);
}

void Scene::KeyDown(Key key)
{
	m_interfaceManager->KeyDown(key);
}

void Scene::KeyUp(Key key)
{
	m_interfaceManager->KeyUp(key);
}

void Scene::Awake()
{
	m_interfaceManager = new Uknitty::InterfaceManager();
}

void Scene::Start()
{
	LoadMapDataFromFile(m_mapId);
	LoadObjectDataFromMap();
	CreateSolidObjectsFromData();
	CreateGround();

	m_interfaceManager->Awake();
	m_interfaceManager->Start();
}

void Scene::Update(float deltaTime)
{
	m_interfaceManager->Update(deltaTime);
}

void Scene::LateUpdate(float deltaTime)
{
	m_interfaceManager->LateUpdate(deltaTime);
}

void Scene::FixedUpdate()
{
	m_interfaceManager->FixedUpdate();
}

void Scene::Destroy()
{
	std::cout << "Destroying Scene: " << m_mapId << std::endl;

	m_interfaceManager->Destroy();

	for(auto& wallData : m_wallDatas)
	{
		delete wallData;
	}
	m_wallDatas.clear();
	m_crate2x4positions.clear();
	m_crate4x4positions.clear();
	m_tankPositions.clear();

	for(Uknitty::Model* model : m_models)
	{
		delete model;
	}

	delete this;
}

void Scene::Draw()
{
	m_interfaceManager->Draw();
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
				m_crate2x4positions.push_back(MAP_CENTER - glm::vec2(object.x / TILE_SIZE, object.y / TILE_SIZE));
			}
		}
		if(objectGroup.name == CRATE_4_X_4_OBJECTGROUP)
		{
			for(tmxparser::TmxObject& object : objectGroup.objects)
			{
				m_crate4x4positions.push_back(MAP_CENTER - glm::vec2(object.x / TILE_SIZE, object.y / TILE_SIZE));
			}
		}
		if(objectGroup.name == TANK_OBJECTGROUP)
		{
			for(tmxparser::TmxObject& object : objectGroup.objects)
			{
				m_tankPositions.push_back(MAP_CENTER - glm::vec2(object.x / TILE_SIZE, object.y / TILE_SIZE));
			}
		}
		if(objectGroup.name == WALL_OBJECTGROUP)
		{
			for(tmxparser::TmxObject& object : objectGroup.objects)
			{
				WallData* wallData = new WallData();
				wallData->position =
					MAP_CENTER
					- glm::vec2((object.x + (object.width / 2.0f)) / TILE_SIZE, (object.y + (object.height / 2.0f)) / TILE_SIZE);
				wallData->size = glm::ivec2(object.width / TILE_SIZE, object.height / TILE_SIZE);
				wallData->wallType = static_cast<WallType>(std::stoi(object.propertyMap["vhu"]));
				m_wallDatas.push_back(wallData);
			}
		}
	}
}

void Scene::CreateSolidObjectsFromData()
{
	Uknitty::Model* crate2x4Model = new Uknitty::Model("../Common/Assets/Models/Crate_2x4/Crate.obj");
	m_models.push_back(crate2x4Model);
	for(auto& crate2x4pos : m_crate2x4positions)
	{
		SolidObject* crate2x4Object = new SolidObject(m_iCamera, crate2x4Model, m_shaderProgram);
		crate2x4Object->GetTransform()->SetPosition(glm::vec3(crate2x4pos.x, 0, crate2x4pos.y));
		m_interfaceManager->AddRender(crate2x4Object);
	}

	Uknitty::Model* crate4x4Model = new Uknitty::Model("../Common/Assets/Models/Crate_4x4/Crate.obj");
	m_models.push_back(crate4x4Model);
	for(auto& crate4x4pos : m_crate4x4positions)
	{
		SolidObject* crate4x4Object = new SolidObject(m_iCamera, crate4x4Model, m_shaderProgram);
		crate4x4Object->GetTransform()->SetPosition(glm::vec3(crate4x4pos.x, 0, crate4x4pos.y));
		m_interfaceManager->AddRender(crate4x4Object);
	}

	Uknitty::Model* tankModel = new Uknitty::Model("../Common/Assets/Models/Tank/Tank.obj");
	m_models.push_back(tankModel);
	for(auto& tankPos : m_tankPositions)
	{
		SolidObject* tankObject = new SolidObject(m_iCamera, tankModel, m_shaderProgram);
		tankObject->GetTransform()->SetPosition(glm::vec3(tankPos.x, 0, tankPos.y));
		m_interfaceManager->AddRender(tankObject);
	}

	for(auto& wallData : m_wallDatas)
	{
		switch(wallData->wallType)
		{
			case WallType::VERTICAL:
			{
				glm::vec3 wallVerticalScale = glm::vec3(wallData->size.x, 1, wallData->size.y);

				Uknitty::Model* wallModel = nullptr;
				if(wallData->size.x == 1)
				{
					wallModel = new Uknitty::Model("../Common/Assets/Models/Wall_1x4x1/Wall_1x4x1.obj", glm::vec2(wallVerticalScale.z, 1));
					m_models.push_back(wallModel);
				}
				else if(wallData->size.x == 2)
				{
					wallModel = new Uknitty::Model("../Common/Assets/Models/Wall_2x4x1/Wall_2x4x1.obj", glm::vec2(wallVerticalScale.z, 1));
					m_models.push_back(wallModel);
				}
				else
				{
					throw std::runtime_error("vertical wall width bigger than 2 is not supported yet");
				}

				SolidObject* wallVerticalObject = new SolidObject(m_iCamera, wallModel, m_shaderProgram);
				wallVerticalObject->GetTransform()->SetScale(glm::vec3(1, 1, wallVerticalScale.z)); // x scale is built in the loaded wallModel
				wallVerticalObject->GetTransform()->SetPosition(glm::vec3(wallData->position.x, 0, wallData->position.y));
				m_interfaceManager->AddRender(wallVerticalObject);
			}

			break;

			case WallType::HORIZONTAL:
			{
				glm::vec3 wallHorizontalScale = glm::vec3(wallData->size.x, 1, wallData->size.y);

				Uknitty::Model* wallModel = nullptr;
				if(wallData->size.y == 1)
				{
					wallModel = new Uknitty::Model("../Common/Assets/Models/Wall_1x4x1/Wall_1x4x1.obj", glm::vec2(wallHorizontalScale.x, 1));
					m_models.push_back(wallModel);
				}
				else if(wallData->size.y == 2)
				{
					wallModel = new Uknitty::Model("../Common/Assets/Models/Wall_1x4x2/Wall_1x4x2.obj", glm::vec2(wallHorizontalScale.x, 1));
					m_models.push_back(wallModel);
				}
				else
				{
					throw std::runtime_error("horizontal wall width bigger than 2 is not supported yet");
				}

				SolidObject* wallHorizontalObject = new SolidObject(m_iCamera, wallModel, m_shaderProgram);
				wallHorizontalObject->GetTransform()->SetScale(glm::vec3(wallHorizontalScale.x, 1, 1)); // z scale is built in the loaded wallModel
				wallHorizontalObject->GetTransform()->SetPosition(glm::vec3(wallData->position.x, 0, wallData->position.y));
				m_interfaceManager->AddRender(wallHorizontalObject);
			}

			break;

			case WallType::UNIFORM:
			{
				glm::vec3 wallUniformScale = glm::vec3(wallData->size.x, 1, wallData->size.y);
				Uknitty::Model* wallUniformModel = new Uknitty::Model("../Common/Assets/Models/Wall_1x4x1/Wall_1x4x1.obj", glm::vec2(wallUniformScale.x, 1));
				m_models.push_back(wallUniformModel);
				SolidObject* wallUnifromObject = new SolidObject(m_iCamera, wallUniformModel, m_shaderProgram);
				wallUnifromObject->GetTransform()->SetScale(wallUniformScale);
				wallUnifromObject->GetTransform()->SetPosition(glm::vec3(wallData->position.x, 0, wallData->position.y));
				m_interfaceManager->AddRender(wallUnifromObject);
			}

			break;

			default:
				throw std::runtime_error("uknown WallType");
		}
	}
}

void Scene::CreateGround()
{
	Uknitty::Model* plane = new Uknitty::Model("../Common/Assets/Models/Primitives/Plane/Plane.obj", glm::vec2(MAP_SCALE_Z, MAP_SCALE_X));
	m_models.push_back(plane);
	SolidObject* planeObject = new SolidObject(m_iCamera, plane, m_shaderProgram);
	planeObject->GetTransform()->SetScale(glm::vec3(MAP_SCALE_X, 0, MAP_SCALE_Z));
	m_interfaceManager->AddRender(planeObject);
}
