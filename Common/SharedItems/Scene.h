#pragma once

#include "AStar.hpp"
#include "Common.h"
#include "glm/glm.hpp"
#include "RoomChangeType.h"
#include "tmxparser.h"
#include <map>
#include <string>

namespace Uknitty
{
class ShaderProgram;
class CameraObject;
class InterfaceManager;
class Model;
}
class GeneralCamera;
class Player;
class btDynamicsWorld;
class SceneManagerBlackboard;

class Scene
{
public:
	Scene(int mapId, Player* player);

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
	};

	GeneralCamera* m_generalCamera = nullptr;
	Player* m_player = nullptr;
	tmxparser::TmxMap m_tmxMap;
	Uknitty::CameraObject* m_iCamera = nullptr;
	Uknitty::ShaderProgram* m_shaderProgram = nullptr;
	Uknitty::InterfaceManager* m_interfaceManager = nullptr;
	const btDynamicsWorld* m_btDynamicsWorld = nullptr;
	AStar::Generator* m_pathFinder = nullptr;
	SceneManagerBlackboard* m_sceneManagerBlackboard = nullptr;

	const std::string MAPS_PATH = "../Common/Assets/Maps/";
	const std::string MAPS_EXTENTION = ".tmx";
	const std::string CRATE_2_X_4_OBJECTGROUP = "crate2x4";
	const std::string CRATE_4_X_4_OBJECTGROUP = "crate4x4";
	const std::string TANK_OBJECTGROUP = "tank";
	const std::string WALL_OBJECTGROUP = "wall";
	const std::string RC_OBJECTGROUP = "rc";
	const std::string FENCE_OBJECTGROUP = "fence";
	const std::string ENEMY_PATROL_OBJECTGROUP = "ep";
	const std::string ASTAR_LAYER_COLLECTION = "astar";
	const int ASTAR_UNWALKABLE_GID = 1;

	int m_mapId = 0;

	std::vector<glm::ivec2> m_crate2x4positions;
	std::vector<glm::ivec2> m_crate4x4positions;
	std::vector<glm::ivec2> m_tankPositions;
	std::vector<glm::ivec2> m_fencePositions;
	std::vector<WallData*> m_wallDatas;
	std::vector<Uknitty::Model*> m_models;
	std::vector<RoomChangeData*> m_roomChangeDatas;
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
};

