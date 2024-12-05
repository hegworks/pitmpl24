#include "Scene.h"

#include "ICamera.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "SolidObject.h"
#include "Transform.h"
#include <iostream>
#include <stdexcept>
#include <tmxparser.h>

Scene::Scene(int mapId, Uknitty::ICamera* camera, Uknitty::ShaderProgram* shaderProgram, Player* player)
{
	m_iCamera = camera;
	m_shaderProgram = shaderProgram;
	m_player = player;

	LoadMapDataFromFile(mapId);
	LoadObjectDataFromMap();
	CreateGround();
}

void Scene::ProcessMousePosition(double xPos, double yPos)
{
}

void Scene::ProcessKeyboard(IKeyboard* iKeyboard)
{
}

void Scene::KeyDown(Key key)
{
}

void Scene::KeyUp(Key key)
{
}

void Scene::Awake()
{
}

void Scene::Start()
{
}

void Scene::Update(float deltaTime)
{
}

void Scene::LateUpdate(float deltaTime)
{
}

void Scene::FixedUpdate()
{
}

void Scene::Destroy()
{
}

void Scene::Draw()
{
}

void Scene::LoadMapDataFromFile(int mapId)
{
	std::string fullPath = MAPS_PATH + std::to_string(mapId) + MAPS_EXTENTION;
	tmxparser::TmxMap map;
	tmxparser::TmxReturn error = tmxparser::parseFromFile(fullPath, &map, MAPS_PATH);
	if(error)
	{
		throw std::runtime_error("parseFromFile failed for file: " + fullPath);
	}

	m_tmxMap = &map;
	std::cout << "YAY" << std::endl;
}

void Scene::LoadObjectDataFromMap()
{
	std::vector<tmxparser::TmxObjectGroup> objectGroups = m_tmxMap->objectGroupCollection;

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
	for(auto& crate2x4pos : m_crate2x4positions)
	{
		SolidObject* crate2x4Object = new SolidObject(m_iCamera, crate2x4Model, m_shaderProgram);
		crate2x4Object->GetTransform()->SetPosition(glm::vec3(crate2x4pos.x, 0, crate2x4pos.y));
		m_renderAbles.push_back(crate2x4Object);
	}

	Uknitty::Model* crate4x4Model = new Uknitty::Model("../Common/Assets/Models/Crate_4x4/Crate.obj");
	for(auto& crate4x4pos : m_crate4x4positions)
	{
		SolidObject* crate4x4Object = new SolidObject(m_iCamera, crate4x4Model, m_shaderProgram);
		crate4x4Object->GetTransform()->SetPosition(glm::vec3(crate4x4pos.x, 0, crate4x4pos.y));
		m_renderAbles.push_back(crate4x4Object);
	}

	Uknitty::Model* tankModel = new Uknitty::Model("../Common/Assets/Models/Tank/Tank.obj");
	for(auto& tankPos : m_tankPositions)
	{
		SolidObject* tankObject = new SolidObject(m_iCamera, tankModel, m_shaderProgram);
		tankObject->GetTransform()->SetPosition(glm::vec3(tankPos.x, 0, tankPos.y));
		m_renderAbles.push_back(tankObject);
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
				}
				else if(wallData->size.x == 2)
				{
					wallModel = new Uknitty::Model("../Common/Assets/Models/Wall_2x4x1/Wall_2x4x1.obj", glm::vec2(wallVerticalScale.z, 1));
				}
				else
				{
					throw std::runtime_error("vertical wall width bigger than 2 is not supported yet");
				}

				SolidObject* wallVerticalObject = new SolidObject(m_iCamera, wallModel, m_shaderProgram);
				wallVerticalObject->GetTransform()->SetScale(glm::vec3(1, 1, wallVerticalScale.z)); // x scale is built in the loaded wallModel
				wallVerticalObject->GetTransform()->SetPosition(glm::vec3(wallData->position.x, 0, wallData->position.y));
				m_renderAbles.push_back(wallVerticalObject);
			}

			break;

			case WallType::HORIZONTAL:
			{
				glm::vec3 wallHorizontalScale = glm::vec3(wallData->size.x, 1, wallData->size.y);

				Uknitty::Model* wallModel = nullptr;
				if(wallData->size.y == 1)
				{
					wallModel = new Uknitty::Model("../Common/Assets/Models/Wall_1x4x1/Wall_1x4x1.obj", glm::vec2(wallHorizontalScale.x, 1));
				}
				else if(wallData->size.y == 2)
				{
					wallModel = new Uknitty::Model("../Common/Assets/Models/Wall_1x4x2/Wall_1x4x2.obj", glm::vec2(wallHorizontalScale.x, 1));
				}
				else
				{
					throw std::runtime_error("horizontal wall width bigger than 2 is not supported yet");
				}

				SolidObject* wallHorizontalObject = new SolidObject(m_iCamera, wallModel, m_shaderProgram);
				wallHorizontalObject->GetTransform()->SetScale(glm::vec3(wallHorizontalScale.x, 1, 1)); // z scale is built in the loaded wallModel
				wallHorizontalObject->GetTransform()->SetPosition(glm::vec3(wallData->position.x, 0, wallData->position.y));
				m_renderAbles.push_back(wallHorizontalObject);
			}

			break;

			case WallType::UNIFORM:
			{
				glm::vec3 wallUniformScale = glm::vec3(wallData->size.x, 1, wallData->size.y);
				Uknitty::Model* wallUniformModel = new Uknitty::Model("../Common/Assets/Models/Wall_1x4x1/Wall_1x4x1.obj", glm::vec2(wallUniformScale.x, 1));
				SolidObject* wallUnifromObject = new SolidObject(m_iCamera, wallUniformModel, m_shaderProgram);
				wallUnifromObject->GetTransform()->SetScale(wallUniformScale);
				wallUnifromObject->GetTransform()->SetPosition(glm::vec3(wallData->position.x, 0, wallData->position.y));
				m_renderAbles.push_back(wallUnifromObject);
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
	SolidObject* planeObject = new SolidObject(m_iCamera, plane, m_shaderProgram);
	planeObject->GetTransform()->SetScale(glm::vec3(MAP_SCALE_X, 0, MAP_SCALE_Z));
	m_renderAbles.push_back(planeObject);
}
