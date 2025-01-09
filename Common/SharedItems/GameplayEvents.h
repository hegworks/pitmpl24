#pragma once

class GameplayEvents
{
public:
	GameplayEvents();

	void OnEnemyBulletHitPlayer();
	void OnPlayerDied();
	void OnPressedQuit();
};
