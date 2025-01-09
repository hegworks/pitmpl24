#pragma once

class UIManager
{
public:
	UIManager();
	~UIManager();

	void Update(float deltaTime);
	void ShowMainMenu() { m_isMainMenuVisible = true; }
	void HideMainMenu() { m_isMainMenuVisible = false; }

private:
	bool m_isMainMenuVisible = false;
};
