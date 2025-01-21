#include "GameManager.h"

#include "ImGui-master/backends/imgui_impl_opengl3.h"
#include "ImGui-master/imgui.h"

#include "CameraObject.h"
#include "Engine.h"
#include "GameplayEvents.h"
#include "GameSharedDependencies.h"
#include "GeneralCamera.h"
#include "IInput.h"
#include "IInputKey.h"
#include "InventoryManager.h"
#include "ModelDataStorage.h"
#include "PhysicsManager.h"
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
	m_generalCamera = static_cast<Uknitty::GeneralCamera*>(m_engine->GetMainCamera());
	m_lastCameraFollowType = m_generalCamera->GetCameraType();

	m_uiManager->ShowMenu(UIManager::MenuType::MAIN_MENU);

	m_gameState = GameState::MAIN_MENU;
	m_iMouse->ReleaseMouseInput();
}

GameManager::~GameManager()
{
	if(m_gameState != GameState::MAIN_MENU)
	{
		delete GameSharedDependencies::Get<SceneManager>();
		delete GameSharedDependencies::Get<InventoryManager>();
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
			if(m_gameState == GameState::GAMEPLAY)
			{
				m_uiManager->ShowMenu(UIManager::MenuType::PAUSE_MENU);
				m_gameState = GameState::PAUSE;
				m_engine->GetMainCamera()->ResetMouseOffset();
				m_iMouse->ReleaseMouseInput();
			}
			else if(m_gameState == GameState::PAUSE)
			{
				m_uiManager->ShowMenu(UIManager::MenuType::HUD);
				m_gameState = GameState::GAMEPLAY;
				m_engine->GetMainCamera()->ResetMouseOffset();
				m_iMouse->CaptureMouseInput();
			}
			break;
		case GameManager::GameEvent::PRESSED_UNPAUSE:
			m_uiManager->ShowMenu(UIManager::MenuType::HUD);
			m_gameState = GameState::GAMEPLAY;
			m_iMouse->CaptureMouseInput();
			break;
		case GameEvent::PRESSED_MAIN_MENU:
			delete GameSharedDependencies::Get<SceneManager>();
			delete GameSharedDependencies::Get<InventoryManager>();
			m_uiManager->ShowMenu(UIManager::MenuType::MAIN_MENU);
			m_gameState = GameState::MAIN_MENU;
			m_iMouse->ReleaseMouseInput();
			break;
		case GameManager::GameEvent::PRESSED_QUIT:
			m_shouldQuit = true;
			break;
		case GameEvent::SHOWED_LOADING_SCREEN:
			m_sceneManager = new SceneManager();
			m_inventoryManager = new InventoryManager();
			m_uiManager->ShowMenu(UIManager::MenuType::HUD);
			m_iMouse->CaptureMouseInput();
			m_gameState = GameState::GAMEPLAY;
			break;
		case GameEvent::PLAYER_COLLIDED_WITH_ROOM_CHANGE:
			m_shouldChangeRoomSoDisablePhysics = true;
			m_uiManager->ShowMenu(UIManager::MenuType::LOADING_SCREEN);
			break;
		case GameEvent::NEW_SCENE_LOADED:
			m_isNewSceneLoadedSoReEnablePhysics = true;
			m_uiManager->ShowMenu(UIManager::MenuType::HUD);
			break;
		case GameEvent::PRESSED_INVENTORY:
			if(m_gameState == GameState::GAMEPLAY)
			{
				m_uiManager->ShowMenu(UIManager::MenuType::INVENTORY);
				m_inventoryManager->ShowInventory();
				m_engine->KeyUpAll();
				m_gameState = GameState::INVENTORY;
				m_iMouse->ReleaseMouseInput();
			}
			else if(m_gameState == GameState::INVENTORY)
			{
				m_uiManager->ShowMenu(UIManager::MenuType::HUD);
				m_inventoryManager->HideInventory();
				m_engine->GetMainCamera()->ResetMouseOffset();
				m_gameState = GameState::GAMEPLAY;
				m_iMouse->CaptureMouseInput();
			}
			break;
		case GameEvent::ONE_ENEMY_ALARMED:
			m_sceneManager->AlarmCurrentScene();
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


	if(m_shouldChangeRoomSoDisablePhysics)
	{
		m_engine->GetPhysicsManager()->Disable();
		m_sceneManager->ChangeScene();
		m_shouldChangeRoomSoDisablePhysics = false;
	}

	if(m_isNewSceneLoadedSoReEnablePhysics)
	{
		m_engine->GetPhysicsManager()->Enable();
		m_isNewSceneLoadedSoReEnablePhysics = false;
	}

	if(m_gameState == GameState::GAMEPLAY || m_gameState == GameState::INVENTORY)
	{
		m_inventoryManager->Update(deltaTime);
		m_sceneManager->Update(deltaTime);
		m_engine->Update(deltaTime);
	}

	if(!m_iMouse->IsCapturingMouseInput())
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos.x = m_iMouse->GetPosition().x;
		io.MousePos.y = m_iMouse->GetPosition().y;
	}

	CheckGeneralCameraTypeChange();
}

void GameManager::KeyDown(Key key)
{
	if(key == Key::ESCAPE)
	{
		TriggerEvent(GameEvent::PRESSED_PAUSE);
	}

	if(key == Key::TAB)
	{
		TriggerEvent(GameEvent::PRESSED_INVENTORY);
	}

	if(key == Key::F)
	{
		m_inventoryManager->UseCurrentItem();
	}

	if(m_gameState == GameState::GAMEPLAY)
	{
		m_engine->GetInstance()->KeyDown(key);
	}
}

void GameManager::KeyUp(Key key)
{
	if(m_gameState == GameState::GAMEPLAY || m_gameState == GameState::INVENTORY)
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

		if(mouseButton == MouseButton::RIGHT)
		{
			m_generalCamera->SetFollowType(Uknitty::GeneralCamera::FollowType::FIRST_PERSON);
		}
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

		if(mouseButton == MouseButton::RIGHT)
		{
			m_generalCamera->SetFollowType(Uknitty::GeneralCamera::FollowType::THIRD_PERSON_FOLLOW);
		}
	}
}

void GameManager::CheckGeneralCameraTypeChange()
{
	if(m_lastCameraFollowType != m_generalCamera->GetCameraType())
	{
		m_lastCameraFollowType = m_generalCamera->GetCameraType();
		GameSharedDependencies::Get<GameplayEvents>()->OnCameraFolowTypeChanged();
	}
}
