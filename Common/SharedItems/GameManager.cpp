#include "GameManager.h"

#include "ImGui-master/backends/imgui_impl_opengl3.h"
#include "ImGui-master/imgui.h"

#include "Engine.h"
#include "GameplayEvents.h"
#include "GameSharedDependencies.h"
#include "IInput.h"
#include "ModelDataStorage.h"
#include "SceneManager.h"
#include "UIManager.h"
#include <stdexcept>

GameManager::GameManager(IMouse* iMouse, IKeyboard* iKeyboard)
{
	GameSharedDependencies::Set<GameManager>(this);

	m_iMouse = iMouse;
	m_iKeyboard = iKeyboard;

	m_engine = Uknitty::Engine::GetInstance();
	m_engine->CreateAndUseDefaultCamera();
	m_engine->InitializeInput(iMouse, iKeyboard);
	m_engine->ValidateBeforeFirstUpdate();

	new GameplayEvents();
	new ModelDataStorage();
	new SceneManager();
	m_uiManager = new UIManager();

	m_uiManager->ShowMainMenu();

	m_gameState = GameState::MAIN_MENU;
}

GameManager::~GameManager()
{
	delete GameSharedDependencies::Get<SceneManager>();
	delete GameSharedDependencies::Get<ModelDataStorage>();
	delete GameSharedDependencies::Get<GameplayEvents>();

	m_engine->Destroy();
}

void GameManager::TriggerEvent(GameEvent gameEvent)
{
	switch(gameEvent)
	{
		case GameManager::GameEvent::PRESSED_START_GAME:
			break;
		case GameManager::GameEvent::PLAYER_DIED:
			break;
		case GameManager::GameEvent::PLAYER_WON:
			break;
		case GameManager::GameEvent::PRESSED_PAUSE:
			break;
		case GameManager::GameEvent::PRESSED_UNPAUSE:
			break;
		case GameManager::GameEvent::PRESSED_QUIT:
			m_shouldQuit = true;
			break;
		default:
			throw std::runtime_error("Invalid gameEvent");
	}
}

void GameManager::Update(float deltaTime)
{
	m_engine->Update(deltaTime);
	m_uiManager->Update(deltaTime);
}

void GameManager::KeyDown(Key key)
{
	m_engine->GetInstance()->KeyDown(key);
}

void GameManager::KeyUp(Key key)
{
	m_engine->GetInstance()->KeyUp(key);
}

void GameManager::MouseButtonDown(MouseButton mouseButton)
{
	ImGuiIO& io = ImGui::GetIO();

	if(!m_iMouse->IsCapturingMouseInput())
	{
		io.AddMouseButtonEvent(ImGuiMouseButton_::ImGuiMouseButton_Left, true);
	}
	else if(!io.WantCaptureMouse)
	{
		m_engine->GetInstance()->MouseButtonDown(mouseButton);
	}
}

void GameManager::MouseButtonUp(MouseButton mouseButton)
{
	ImGuiIO& io = ImGui::GetIO();

	if(!m_iMouse->IsCapturingMouseInput())
	{
		io.AddMouseButtonEvent(ImGuiMouseButton_::ImGuiMouseButton_Left, false);
	}
	else if(!io.WantCaptureMouse)
	{
		m_engine->GetInstance()->MouseButtonUp(mouseButton);
	}
}
