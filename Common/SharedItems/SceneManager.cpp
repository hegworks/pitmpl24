#include "SceneManager.h"

#include "GeneralCamera.h"
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
	m_generalCamera = new GeneralCamera();
	m_flowInputAbles.push_back(m_generalCamera);

	Uknitty::ShaderProgram* shaderProgram = new Uknitty::ShaderProgram("../Common/Assets/Shaders/Vertex.glsl", "../Common/Assets/Shaders/Fragment.glsl");

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
	m_currentScene = new Scene(mapId);
}
