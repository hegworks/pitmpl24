#include "SharedEvents.h"

#include "Player.h"

void SharedEvents::OnEnemyBulletHitPlayer()
{
	m_player->OnEnemyBulletHit();
}
