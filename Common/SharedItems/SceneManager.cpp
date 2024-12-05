#include "SceneManager.h"

#include "GeneralCamera.h"
#include "Model.h"
#include "Player.h"
#include "Scene.h"
#include "ShaderProgram.h"

void SceneManager::ProcessMousePosition(double xPos, double yPos)
{
}

void SceneManager::ProcessKeyboard(IKeyboard* iKeyboard)
{
}

void SceneManager::KeyDown(Key key)
{
}

void SceneManager::KeyUp(Key key)
{
}

void SceneManager::Awake()
{
}

void SceneManager::Start()
{
	CreatePlayer();
	CreateCamera();
	static_cast<GeneralCamera*>(m_camera)->SetFollowTransform(m_player->m_transform);
	CreateShaderProgram();
	LoadScene(INITIAL_MAP_ID);
}

void SceneManager::Update(float deltaTime)
{
}

void SceneManager::LateUpdate(float deltaTime)
{
}

void SceneManager::FixedUpdate()
{
}

void SceneManager::Destroy()
{
}

void SceneManager::Draw()
{
}

void SceneManager::LoadScene(int mapId)
{
	m_currentScene = new Scene(mapId, m_camera, m_shaderProgram, m_player);
}

void SceneManager::CreatePlayer()
{
	Uknitty::Model* snake = new Uknitty::Model("../Common/Assets/Models/NakedSnake/NakedSnake.obj");
	m_player = new Player(snake, m_camera, m_shaderProgram);
	m_flowInputRenderAbles.push_back(m_player);
}

void SceneManager::CreateCamera()
{
	m_camera = new GeneralCamera();
	m_flowInputAbles.push_back(m_camera);
}

void SceneManager::CreateShaderProgram()
{
	m_shaderProgram = new Uknitty::ShaderProgram("../Common/Assets/Shaders/Vertex.glsl", "../Common/Assets/Shaders/Fragment.glsl");
}
