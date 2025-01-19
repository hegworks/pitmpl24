#pragma once

#include "GeneralCamera.h"
#include "IInput.h"
#include "IInputKey.h"

namespace Uknitty
{
class Engine;
}
class UIManager;
class InventoryManager;
class SceneManager;

class GameManager
{
public:
	GameManager(IMouse* iMouse, IKeyboard* iKeyboard);
	~GameManager();

	enum class GameState
	{
		MAIN_MENU,
		GAMEPLAY,
		PAUSE,
		WIN,
		LOSE,
		LOADING,
		INVENTORY,
	};

	enum class GameEvent
	{
		PRESSED_START_GAME,
		PLAYER_DIED,
		PLAYER_WON,
		PRESSED_PAUSE,
		PRESSED_UNPAUSE,
		PRESSED_QUIT,
		PRESSED_MAIN_MENU,
		SHOWED_LOADING_SCREEN,
		PLAYER_COLLIDED_WITH_ROOM_CHANGE,
		NEW_SCENE_LOADED,
		PRESSED_INVENTORY,
	};

	void TriggerEvent(GameEvent gameEvent);
	void Update(float deltaTime);
	bool ShouldQuit() const { return m_shouldQuit; }

	void KeyDown(Key key);
	void KeyUp(Key key);
	void MouseButtonDown(MouseButton mouseButton);
	void MouseButtonUp(MouseButton mouseButton);

private:
	void CheckGeneralCameraTypeChange();

	IMouse* m_iMouse = nullptr;
	IKeyboard* m_iKeyboard = nullptr;
	Uknitty::Engine* m_engine = nullptr;
	UIManager* m_uiManager = nullptr;
	SceneManager* m_sceneManager = nullptr;
	InventoryManager* m_inventoryManager = nullptr;
	Uknitty::GeneralCamera* m_generalCamera = nullptr;

	GameState m_gameState = GameState::MAIN_MENU;
	bool m_shouldQuit = false;
	bool m_hasWaitedFor1FrameToShowLoadingScreen = false;
	bool m_shouldChangeRoomSoDisablePhysics = false;
	bool m_isNewSceneLoadedSoReEnablePhysics = false;
	Uknitty::GeneralCamera::FollowType m_lastCameraFollowType;
};
