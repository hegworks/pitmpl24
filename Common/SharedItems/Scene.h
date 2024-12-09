#pragma once

#include "Common.h"
#include "glm/glm.hpp"
#include "Interfaces.h"
#include "RoomChangeType.h"
#include "tmxparser.h"
#include <map>
#include <string>

namespace Uknitty
{
class ShaderProgram;
class ICamera;
class InterfaceManager;
class Model;
}
class GeneralCamera;
class Player;
class btDynamicsWorld;

class Scene : public Uknitty::FlowInputRender
{
public:
	Scene(int mapId, Uknitty::ICamera* iCamera, Uknitty::ShaderProgram* shaderProgram, Player* player, btDynamicsWorld* btDynamicsWorld);

	// Inherited via FlowInputRender
	virtual void ProcessMousePosition(double xPos, double yPos) override;
	virtual void ProcessKeyboard(IKeyboard* iKeyboard) override;
	virtual void KeyDown(Key key) override;
	virtual void KeyUp(Key key) override;
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void LateUpdate(float deltaTime) override;
	virtual void FixedUpdate() override;
	virtual void Destroy() override;
	virtual void Draw() override;

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
	Uknitty::ICamera* m_iCamera = nullptr;
	Uknitty::ShaderProgram* m_shaderProgram = nullptr;
	Uknitty::InterfaceManager* m_interfaceManager = nullptr;
	btDynamicsWorld* m_btDynamicsWorld = nullptr;

	const std::string MAPS_PATH = "../Common/Assets/Maps/";
	const std::string MAPS_EXTENTION = ".tmx";
	const int TILE_SIZE = 32;
	const glm::vec2 MAP_CENTER = glm::vec2(MAP_SCALE_X / 2, MAP_SCALE_Z / 2);
	const std::string CRATE_2_X_4_OBJECTGROUP = "crate2x4";
	const std::string CRATE_4_X_4_OBJECTGROUP = "crate4x4";
	const std::string TANK_OBJECTGROUP = "tank";
	const std::string WALL_OBJECTGROUP = "wall";
	const std::string RC_OBJECTGROUP = "rc";
	const std::string FENCE_OBJECTGROUP = "fence";
	const std::string ENEMY_PATROL_OBJECTGROUP = "ep";

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
};

