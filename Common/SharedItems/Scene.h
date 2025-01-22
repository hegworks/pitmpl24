#pragma once

#include "AStar.hpp"
#include "GameSettings.h"
#include "glm/glm.hpp"
#include "RoomChangeType.h"
#include "tmxparser.h"
#include "UknittySettings.h"
#include "unordered_map"
#include <map>
#include <string>
#include <vector>

namespace Uknitty
{
class ShaderProgram;
class CameraObject;
class InterfaceManager;
class Model;
class Engine;
class GameObject;
}
class GeneralCamera;
class Player;
class btDynamicsWorld;
class SceneManagerBlackboard;
class ModelDataStorage;
class Enemy;
class PerlinNoiseManager;
class PerlinCRender;

class Scene
{
public:
	Scene(int mapId);
	~Scene();

	void EnterAlarmState();
	void Update(float deltaTime);

private:
	enum class WallType
	{
		VERTICAL,
		HORIZONTAL,
		UNIFORM,
	};

	struct WallData
	{
		glm::vec2 position;
		glm::ivec2 size;
		WallType wallType;
	};

	struct RoomChangeData
	{
		glm::vec2 position;
		glm::ivec2 size;
		RoomChangeType roomChangeType;
		Uknitty::UserPointerData* userPointerData = nullptr;
	};

	Player* m_player = nullptr;
	tmxparser::TmxMap m_tmxMap;
	AStar::Generator* m_pathFinder = nullptr;
	SceneManagerBlackboard* m_sceneManagerBlackboard = nullptr;
	ModelDataStorage* m_modelDataStorage = nullptr;
	Uknitty::Engine* m_engine = nullptr;
	Uknitty::ShaderProgram* m_shaderProgram = nullptr;
	std::vector<glm::vec3>* m_patrolPositionsVector = nullptr;
	PerlinNoiseManager* m_perlinNoiseManager = nullptr;
	Uknitty::GameObject* m_perlinTextureObject = nullptr;
	Uknitty::GameObject* m_perlinMeshObject = nullptr;
	PerlinCRender* m_perlinTextureCRender = nullptr;
	PerlinCRender* m_perlinMeshCRender = nullptr;

	const std::string MAPS_PATH = "../Common/Assets/Maps/";
	const std::string MAPS_EXTENTION = ".tmx";
	const std::string CRATE_2_X_4_OBJECTGROUP = "crate2x4";
	const std::string CRATE_4_X_4_OBJECTGROUP = "crate4x4";
	const std::string TANK_OBJECTGROUP = "tank";
	const std::string WALL_OBJECTGROUP = "wall";
	const std::string RC_OBJECTGROUP = "rc";
	const std::string FENCE_OBJECTGROUP = "fence";
	const std::string WIN_OBJECTGROUP = "win";
	const std::string ENEMY_PATROL_OBJECTGROUP = "ep";
	const std::string ASTAR_LAYER_COLLECTION = "astar";
	const unsigned int ASTAR_UNWALKABLE_GID = 1;
	const int PERLIN_MAP_ID = 0;

	int m_mapId = 0;
	bool m_isInAlarmState = false;

	/// <summary>
	/// key in ModelDataStorage, position
	/// </summary>
	std::unordered_multimap <std::string, glm::ivec2> m_staticObjectsPositions;
	std::vector<WallData*> m_wallDatas;
	std::vector<RoomChangeData*> m_roomChangeDatas;

	std::vector<Uknitty::GameObject*> m_createdGameObjects;
	std::vector<Enemy*> m_enemies;

	/// <summary>
	/// enemyIndex, patrolPositionIndex, patrolPosition
	/// </summary>
	std::map<int, std::map<int, glm::vec3>> m_enemiesPatrolPositions;

	void LoadMapDataFromFile(int mapId);
	void LoadObjectDataFromMap();
	void CreateSolidObjectsFromData();
	void CreateGround();
	void CreatePathFinder();
	void CreateEnemies();
	void GeneratePerlinTexture();
	void UpdatePerlinTexture(float deltaTime);
};

