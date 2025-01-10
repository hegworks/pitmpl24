#include "GameManager.h"

#include "ImGui-master/backends/imgui_impl_opengl3.h"
#include "ImGui-master/imgui.h"

#include "Engine.h"
#include "GameplayEvents.h"
#include "GameSharedDependencies.h"
#include "IInput.h"
#include "IInputKey.h"
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
	m_uiManager = new UIManager();

	m_uiManager->ShowMenu(UIManager::MenuType::MAIN_MENU);

	m_gameState = GameState::MAIN_MENU;
	m_iMouse->ReleaseMouseInput();
}

GameManager::~GameManager()
{
	if(m_gameState != GameState::MAIN_MENU)
	{
		delete GameSharedDependencies::Get<SceneManager>();
	}
	delete GameSharedDependencies::Get<ModelDataStorage>();
	delete GameSharedDependencies::Get<GameplayEvents>();

	m_engine->Destroy();
}

void GameManager::TriggerEvent(GameEvent gameEvent)
{
	switch(gameEvent)
	{
		case GameManager::GameEvent::PRESSED_START_GAME:
			m_uiManager->ShowMenu(UIManager::MenuType::NONE);
			new SceneManager();
			m_iMouse->CaptureMouseInput();
			m_gameState = GameState::GAMEPLAY;
			break;
		case GameManager::GameEvent::PLAYER_DIED:
			m_uiManager->ShowMenu(UIManager::MenuType::LOSE_MENU);
			m_gameState = GameState::LOSE;
			m_iMouse->ReleaseMouseInput();
			break;
		case GameManager::GameEvent::PLAYER_WON:
			m_uiManager->ShowMenu(UIManager::MenuType::WIN_MENU);
			m_gameState = GameState::WIN;
			m_iMouse->ReleaseMouseInput();
			break;
		case GameManager::GameEvent::PRESSED_PAUSE:
			if(m_gameState == GameState::MAIN_MENU) break;
			m_uiManager->ShowMenu(UIManager::MenuType::PAUSE_MENU);
			m_gameState = GameState::PAUSE;
			m_iMouse->ReleaseMouseInput();
			break;
		case GameManager::GameEvent::PRESSED_UNPAUSE:
			m_uiManager->ShowMenu(UIManager::MenuType::NONE);
			m_gameState = GameState::GAMEPLAY;
			m_iMouse->CaptureMouseInput();
			break;
		case GameEvent::PRESSED_MAIN_MENU:
			delete GameSharedDependencies::Get<SceneManager>();
			m_uiManager->ShowMenu(UIManager::MenuType::MAIN_MENU);
			m_gameState = GameState::MAIN_MENU;
			m_iMouse->ReleaseMouseInput();
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
	if(m_gameState == GameState::GAMEPLAY)
	{
		m_engine->Update(deltaTime);
	}

	m_uiManager->Update(deltaTime);
}

void GameManager::KeyDown(Key key)
{
	if(key == Key::ESCAPE)
	{
		TriggerEvent(GameEvent::PRESSED_PAUSE);
	}

	if(m_gameState == GameState::GAMEPLAY)
	{
		m_engine->GetInstance()->KeyDown(key);
	}
}

void GameManager::KeyUp(Key key)
{
	if(m_gameState == GameState::GAMEPLAY)
	{
		m_engine->GetInstance()->KeyUp(key);
	}
}

void GameManager::MouseButtonDown(MouseButton mouseButton)
{
	ImGuiIO& io = ImGui::GetIO();

	if(!m_iMouse->IsCapturingMouseInput())
	{
		io.AddMouseButtonEvent(ImGuiMouseButton_::ImGuiMouseButton_Left, true);
	}
	else if(!io.WantCaptureMouse && m_gameState == GameState::GAMEPLAY)
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
	else if(!io.WantCaptureMouse && m_gameState == GameState::GAMEPLAY)
	{
		m_engine->GetInstance()->MouseButtonUp(mouseButton);
	}
}
