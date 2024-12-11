#pragma once

#include "btBulletDynamicsCommon.h"
#include "IInputKey.h"
#include "Interfaces.h"
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

class SceneManager : public Uknitty::FlowInputRender
{
public:
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
	Scene* m_currentScene = nullptr;
	GeneralCamera* m_camera = nullptr;
	Player* m_player = nullptr;
	Uknitty::ShaderProgram* m_shaderProgram = nullptr;
	Uknitty::InterfaceManager* m_interfaceManager = nullptr;
	RoomFinder* m_roomFinder = nullptr;

	btDefaultCollisionConfiguration* m_btCollisionConfiguration = nullptr;
	btDbvtBroadphase* m_btBroadphase = nullptr;
	btCollisionDispatcher* m_btDispatcher = nullptr;
	btSequentialImpulseConstraintSolver* m_btSolver = nullptr;
	Uknitty::BTDebugDraw* m_btDebugDrawer = nullptr;
	btDiscreteDynamicsWorld* m_btDynamicsWorld = nullptr;
	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	btAlignedObjectArray<btCollisionShape*> m_btCollisionShapes;
	Uknitty::CollisionManager* m_collisionManager = nullptr;

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
