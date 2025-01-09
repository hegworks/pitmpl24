#pragma once

#include "IInput.h"
#include "IInputKey.h"

namespace Uknitty
{
class Engine;
}
class UIManager;

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
	};

	enum class GameEvent
	{
		PRESSED_START_GAME,
		PLAYER_DIED,
		PLAYER_WON,
		PRESSED_PAUSE,
		PRESSED_UNPAUSE,
		PRESSED_QUIT,
	};

	void TriggerEvent(GameEvent gameEvent);
	void Update(float deltaTime);
	bool ShouldQuit() const { return m_shouldQuit; }

	void KeyDown(Key key);
	void KeyUp(Key key);
	void MouseButtonDown(MouseButton mouseButton);
	void MouseButtonUp(MouseButton mouseButton);

private:
	IMouse* m_iMouse = nullptr;
	IKeyboard* m_iKeyboard = nullptr;
	Uknitty::Engine* m_engine = nullptr;
	UIManager* m_uiManager = nullptr;

	GameState m_gameState = GameState::MAIN_MENU;
	bool m_shouldQuit = false;
};
