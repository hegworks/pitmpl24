#include "SceneManager.h"

#include "GeneralCamera.h"
#include "InterfaceManager.h"
#include "Model.h"
#include "Player.h"
#include "Scene.h"
#include "ShaderProgram.h"

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
	if(key == CHANGE_SCENE_KEY)
	{
		m_currentScene->Destroy();
		m_interfaceManager->RemoveFlowInputRender(m_currentScene);
		m_currentMapId = m_currentMapId == 1 ? 2 : 1;
		LoadScene(m_currentMapId);
		m_interfaceManager->AddFlowInputRender(m_currentScene);
		m_currentScene->Awake();
		m_currentScene->Start();
	}

	else
	{
		m_interfaceManager->KeyDown(key);
	}
}

void SceneManager::KeyUp(Key key)
{
	m_interfaceManager->KeyUp(key);
}

void SceneManager::Awake()
{
	m_interfaceManager = new Uknitty::InterfaceManager();
}

void SceneManager::Start()
{
	CreateCamera();
	m_interfaceManager->AddFlowInput(m_camera);

	CreateShaderProgram();

	CreatePlayer();
	m_interfaceManager->AddFlowInputRender(m_player);

	static_cast<GeneralCamera*>(m_camera)->SetFollowTransform(m_player->m_transform);

	LoadScene(INITIAL_MAP_ID);
	m_interfaceManager->AddFlowInputRender(m_currentScene);

	m_interfaceManager->Awake();
	m_interfaceManager->Start();
}

void SceneManager::Update(float deltaTime)
{
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
	delete m_currentScene;
	delete m_player;
	delete m_shaderProgram;
	delete m_camera;
	delete m_interfaceManager;
	m_interfaceManager->Destroy();
}

void SceneManager::Draw()
{
	m_interfaceManager->Draw();
}

void SceneManager::LoadScene(int mapId)
{
	m_currentScene = new Scene(mapId, m_camera, m_shaderProgram, m_player);
}

void SceneManager::CreatePlayer()
{
	Uknitty::Model* snake = new Uknitty::Model("../Common/Assets/Models/NakedSnake/NakedSnake.obj");
	m_player = new Player(snake, m_camera, m_shaderProgram);
}

void SceneManager::CreateCamera()
{
	m_camera = new GeneralCamera();
}

void SceneManager::CreateShaderProgram()
{
	m_shaderProgram = new Uknitty::ShaderProgram("../Common/Assets/Shaders/Vertex.glsl", "../Common/Assets/Shaders/Fragment.glsl");
}
