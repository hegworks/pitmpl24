#pragma once

namespace Uknitty
{
class CountdownTimer;
}

class UIManager
{
public:
	UIManager();
	~UIManager() = default;

	enum class MenuType
	{
		NONE,
		MAIN_MENU,
		PAUSE_MENU,
		WIN_MENU,
		LOSE_MENU,
		LOADING_SCREEN,
		HUD,
		INVENTORY,
	};

	void Update(float deltaTime);
	void ShowMenu(MenuType menuType) { m_currentMenu = menuType; }
	void UpdateFPS(int fps);
	void PlayRedScreenEffect();
	void PlayHitMarkerEffect();

private:
	const float RED_SCREEN_EFFECT_DURATION = 0.35f;
	const float HIT_MARKER_EFFECT_DURATION = 0.15f;

	bool m_isMainMenuVisible = false;
	MenuType m_currentMenu = MenuType::MAIN_MENU;
	int m_fps = 0;
	Uknitty::CountdownTimer* m_redScreenEffectTimer = nullptr;
	Uknitty::CountdownTimer* m_hitMarkerEffectTimer = nullptr;

	void MainMenu();
	void PauseMenu();
	void LoseMenu();
	void WinMenu();
	void LoadingScreen();
	void HUD();
	void FPSCounter();
	void Inventory();
	void UpdateRedScreenEffect();
	void UpdateHitMarkerEffect();
};
