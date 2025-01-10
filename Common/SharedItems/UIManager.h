#pragma once

class UIManager
{
public:
	UIManager();
	~UIManager();

	enum class MenuType
	{
		NONE,
		MAIN_MENU,
		PAUSE_MENU,
		WIN_MENU,
		LOSE_MENU,
	};

	void Update(float deltaTime);
	void ShowMenu(MenuType menuType) { m_currentMenu = menuType; }

private:
	bool m_isMainMenuVisible = false;
	MenuType m_currentMenu = MenuType::MAIN_MENU;

	void MainMenu();
	void PauseMenu();
	void LoseMenu();
	void WinMenu();
};
