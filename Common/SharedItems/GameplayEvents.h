#pragma once

class GameplayEvents
{
public:
	GameplayEvents();

	void OnEnemyBulletHitPlayer();
	void OnPlayerDied();
	void OnPressedQuit();
	void OnPressedMainMenu();
	void OnPressedPause();
	void OnPressedUnpause();
	void OnPressedStartGame();
	void OnPlayerWon();
	void OnPlayerCollidedWithRoomChange();
	void OnNewSceneLoaded();
};
