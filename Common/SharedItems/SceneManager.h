#pragma once

#include "btBulletDynamicsCommon.h"
#include "GameSettings.h"
#include "IInputKey.h"
#include "RoomChangeType.h"
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

private:
	Scene* m_currentScene = nullptr;
	Player* m_player = nullptr;
	RoomFinder* m_roomFinder = nullptr;
	Uknitty::Engine* m_engine = nullptr;
	ModelDataStorage* m_modelDataStorage = nullptr;
	SceneManagerBlackboard* m_sceneManagerBlackboard = nullptr;

	const std::string VERTEX_SHADER_PATH = "../Common/Assets/Maps/";
	const std::string FRAGMENT_SHADER_PATH = "../Common/Assets/Maps/";
	const Key CHANGE_SCENE_KEY = Key::C;

	int m_currentMapId = STARTING_MAP_ID;

	void OnPlayerCollidedWithRoomChange(RoomChangeType roomChangeType);

	void LoadScene(int mapId);
	void CreatePlayer();
	void CreateShaderProgram();
};
