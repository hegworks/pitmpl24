#include "GameplayEvents.h"

#include "GameManager.h"
#include "GameSharedDependencies.h"
#include "Player.h"

GameplayEvents::GameplayEvents()
{
	GameSharedDependencies::Set<GameplayEvents>(this);
}

void GameplayEvents::OnEnemyBulletHitPlayer()
{
	GameSharedDependencies::Get<Player>()->OnEnemyBulletHit();
}

void GameplayEvents::OnPlayerDied()
{
	GameSharedDependencies::Get<GameManager>()->TriggerEvent(GameManager::GameEvent::PLAYER_DIED);
}
