#pragma once

#include "btBulletDynamicsCommon.h"
#include "GameSettings.h"
#include "IInputKey.h"
#include "RoomChangeType.h"
#include <array>
#include <string>
#include <vector>

namespace Uknitty
{
class ShaderProgram;
class InterfaceManager;
class Model;
class BTDebugDraw;
class CollisionManager;
class Engine;
class ModelObject;
class LightObject;
class CountdownTimer;
}
class Scene;
class GeneralCamera;
class Player;
class RoomFinder;
class SceneManagerBlackboard;
class ModelDataStorage;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void ChangeScene();
	void Update(float deltaTime);

private:
	Scene* m_currentScene = nullptr;
	Player* m_player = nullptr;
	Uknitty::LightObject* m_centralSpotLight = nullptr;
	Uknitty::LightObject* m_dirLight = nullptr;
	Uknitty::ModelObject* m_centralSpotLightModel = nullptr;
	std::array<Uknitty::LightObject*, 2> m_pointLights;
	std::array<glm::vec3, 6> m_pointLightPossibleColors;
	std::array<glm::vec3, 20> m_pointLightGenratedColors;
	RoomFinder* m_roomFinder = nullptr;
	Uknitty::Engine* m_engine = nullptr;
	ModelDataStorage* m_modelDataStorage = nullptr;
	SceneManagerBlackboard* m_sceneManagerBlackboard = nullptr;
	Uknitty::CountdownTimer* m_pointLightFlickeringTimer = nullptr;
	bool m_isTimerForTurningOff = true;
	int m_flickeringLightIndex = 0;
	glm::vec3 m_colorBeforeFlickering = glm::vec3(0);

	const std::string VERTEX_SHADER_PATH = "../Common/Assets/Maps/";
	const std::string FRAGMENT_SHADER_PATH = "../Common/Assets/Maps/";
	const Key CHANGE_SCENE_KEY = Key::C;

	int m_currentMapId = STARTING_MAP_ID;

	void OnPlayerCollidedWithRoomChange(RoomChangeType roomChangeType);
	void UpdateCentralSpotLight(float deltaTime);
	void UpdatePointLightsFlickering(float deltaTime);
	void ChangePointLightColorsInNewRoom();
	void LoadScene(int mapId);
	void LoadAllScenes();
	void GenerateRandomLights();
	void CreatePlayer();
	void CreateLights();
	void CreateShaderProgram();
};
