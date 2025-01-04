#include "SceneManager.h"

#include "btBulletDynamicsCommon.h"
#include "CollisionManager.h"
#include "GameSettings.h"
#include "GeneralCamera.h"
#include "InterfaceManager.h"
#include "Model.h"
#include "Player.h"
#include "RoomFinder.h"
#include "Scene.h"
#include "SceneManagerBlackboard.h"
#include "ShaderProgram.h"
#include "SharedDependencies.h"
#include <BTDebugDraw.h>

#if 0 //TODO GOH
void SceneManager::ProcessMousePosition(double xPos, double yPos)
{
	m_interfaceManager->ProcessMousePosition(xPos, yPos);
}

void SceneManager::ProcessKeyboard(IKeyboard* iKeyboard)
{
	m_interfaceManager->ProcessKeyboard(iKeyboard);
}

void SceneManager::KeyDown(Key key)
{
	/*if(key == CHANGE_SCENE_KEY)
	{
		m_currentScene->Destroy();
		m_interfaceManager->RemoveFlowInputRender(m_currentScene);
		m_currentMapId = m_currentMapId == 1 ? 2 : 1;
		LoadScene(m_currentMapId);
		m_interfaceManager->AddFlowInputRender(m_currentScene);
		m_currentScene->Awake();
		m_currentScene->Start();
	}*/

	//else
	//{
	m_interfaceManager->KeyDown(key);
	//}
}

void SceneManager::KeyUp(Key key)
{
	m_interfaceManager->KeyUp(key);
}

void SceneManager::Awake()
{
	m_interfaceManager = new Uknitty::InterfaceManager();
	m_roomFinder = new RoomFinder();
}

void SceneManager::Start()
{
	CreateCamera();
	m_interfaceManager->AddFlowInput(m_camera);

	CreateShaderProgram();

	CreatePhysicsWorld();

	CreatePlayer();
#if 0 //TODO GOH
	m_collisionManager->RegisterListener(m_player->GetPhysics()->GetRigidBody(), m_player->GetPhysics());
#endif
	m_interfaceManager->AddFlowInputRender(m_player);
	m_player->SetCollidedWithRoomChangeCallback([this](RoomChangeType roomChangeType) { OnPlayerCollidedWithRoomChange(roomChangeType); });

	static_cast<GeneralCamera*>(m_camera)->SetFollowTransform(m_player->GetTransform());

	LoadScene(m_roomFinder->GetCurrentLevelId());
	m_interfaceManager->AddFlowInputRender(m_currentScene);

	m_interfaceManager->Awake();
	m_interfaceManager->Start();
}

void SceneManager::Update(float deltaTime)
{
	UpdatePhysics(deltaTime);
	m_interfaceManager->Update(deltaTime);
}

void SceneManager::LateUpdate(float deltaTime)
{
	m_interfaceManager->LateUpdate(deltaTime);
}

void SceneManager::FixedUpdate()
{
	m_interfaceManager->FixedUpdate();
}

void SceneManager::Destroy()
{
	std::cout << "Destroying SceneManager" << std::endl;
	m_interfaceManager->Destroy();
	DestroyPhysics();
	delete m_shaderProgram;
	delete m_snakeModel;
	delete this;
}

void SceneManager::Draw()
{
	m_interfaceManager->Draw();
#ifdef DEBUG_DRAW_PHYSICS
	m_btDynamicsWorld->debugDrawWorld();
	m_btDebugDrawer->flushLines();
#endif // DEBUG_DRAW_PHYSICS
}

void SceneManager::OnPlayerCollidedWithRoomChange(RoomChangeType roomChangeType)
{
	std::cout << "Player collided with room change: " << static_cast<int>(roomChangeType) << std::endl;

	m_isNewSceneLoading = true;
	RoomChange* newRoomChange = m_roomFinder->FindNextRoom(roomChangeType);
	m_player->RoomChangedSetPosition(newRoomChange);
	int newMapId = newRoomChange->nextRoomId;
	ChangeScene(newMapId);
	m_isNewSceneLoading = false;
	m_roomFinder->SetCurrentLevelId(newMapId);
}

void SceneManager::LoadScene(int mapId)
{
	m_currentScene = new Scene(mapId, m_player);
}

void SceneManager::ChangeScene(int mapId)
{
	m_currentScene->Destroy();
	m_interfaceManager->RemoveFlowInputRender(m_currentScene);
	m_currentMapId = mapId;
	LoadScene(m_currentMapId);
	m_interfaceManager->AddFlowInputRender(m_currentScene);
	m_currentScene->Awake();
	m_currentScene->Start();
}

void SceneManager::CreatePlayer()
{
	m_snakeModel = new Uknitty::Model("../Common/Assets/Models/NakedSnake/NakedSnake.obj", m_shaderProgram);
	m_player = new Player(m_snakeModel);
}

void SceneManager::CreateCamera()
{
	m_camera = new GeneralCamera();
	SharedDependencies::SetCamera(m_camera);
}

void SceneManager::CreateShaderProgram()
{
	m_shaderProgram = new Uknitty::ShaderProgram("../Common/Assets/Shaders/Vertex.glsl", "../Common/Assets/Shaders/Fragment.glsl");
	SharedDependencies::SetShaderProgram(m_shaderProgram);
}

void SceneManager::CreatePhysicsWorld()
{
}

void SceneManager::UpdatePhysics(float deltaTime)
{

}

void SceneManager::DestroyPhysics()
{

}
#endif
