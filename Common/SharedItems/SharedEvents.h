#pragma once

class Player;

class SharedEvents
{
public:
	void SetPlayer(Player* player) { m_player = player; }

	void OnEnemyBulletHitPlayer();

private:
	Player* m_player = nullptr;
};
