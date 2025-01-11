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
			m_uiManager->ShowMenu(UIManager::MenuType::LOADING_SCREEN);
			m_gameState = GameState::LOADING;
			m_hasWaitedFor1FrameToShowLoadingScreen = false;
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
			if(m_gameState == GameState::MAIN_MENU || m_gameState == GameState::LOSE || m_gameState == GameState::WIN) break;
			m_uiManager->ShowMenu(UIManager::MenuType::PAUSE_MENU);
			m_gameState = GameState::PAUSE;
			m_iMouse->ReleaseMouseInput();
			break;
		case GameManager::GameEvent::PRESSED_UNPAUSE:
			m_uiManager->ShowMenu(UIManager::MenuType::HUD);
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
		case GameEvent::SHOWED_LOADING_SCREEN:
			new SceneManager();
			m_uiManager->ShowMenu(UIManager::MenuType::HUD);
			m_iMouse->CaptureMouseInput();
			m_gameState = GameState::GAMEPLAY;
			break;
		default:
			throw std::runtime_error("Invalid gameEvent");
	}
}

void GameManager::Update(float deltaTime)
{
	if(m_gameState == GameState::LOADING && m_hasWaitedFor1FrameToShowLoadingScreen)
	{
		TriggerEvent(GameEvent::SHOWED_LOADING_SCREEN);
		m_hasWaitedFor1FrameToShowLoadingScreen = false;
	}
	m_hasWaitedFor1FrameToShowLoadingScreen = true;

	if(m_gameState == GameState::GAMEPLAY)
	{
		m_engine->Update(deltaTime);
	}

	if(!m_iMouse->IsCapturingMouseInput())
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos.x = m_iMouse->GetPosition().x;
		io.MousePos.y = m_iMouse->GetPosition().y;
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
