#pragma once

#include "btBulletDynamicsCommon.h"
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
}
class Scene;
class GeneralCamera;
class Player;
class RoomFinder;
class SceneManagerBlackboard;

class SceneManager
{
public:

private:
	Scene* m_currentScene = nullptr;
	GeneralCamera* m_camera = nullptr;
	Player* m_player = nullptr;
	Uknitty::ShaderProgram* m_shaderProgram = nullptr;
	Uknitty::InterfaceManager* m_interfaceManager = nullptr;
	RoomFinder* m_roomFinder = nullptr;

	SceneManagerBlackboard* m_sceneManagerBlackboard = nullptr;

	Uknitty::Model* m_snakeModel = nullptr;

	const int INITIAL_MAP_ID = 1;
	const std::string VERTEX_SHADER_PATH = "../Common/Assets/Maps/";
	const std::string FRAGMENT_SHADER_PATH = "../Common/Assets/Maps/";
	const Key CHANGE_SCENE_KEY = Key::C;

	int m_currentMapId = INITIAL_MAP_ID;
	bool m_isNewSceneLoading = false;

	void OnPlayerCollidedWithRoomChange(RoomChangeType roomChangeType);

	void LoadScene(int mapId);
	void ChangeScene(int mapId);
	void CreatePlayer();
	void CreateCamera();
	void CreateShaderProgram();
	void CreatePhysicsWorld();
	void UpdatePhysics(float deltaTime);
	void DestroyPhysics();
};
