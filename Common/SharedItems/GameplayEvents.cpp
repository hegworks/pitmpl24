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

void GameplayEvents::OnPressedQuit()
{
	GameSharedDependencies::Get<GameManager>()->TriggerEvent(GameManager::GameEvent::PRESSED_QUIT);
}

void GameplayEvents::OnPressedMainMenu()
{
	GameSharedDependencies::Get<GameManager>()->TriggerEvent(GameManager::GameEvent::PRESSED_MAIN_MENU);
}

void GameplayEvents::OnPressedPause()
{
	GameSharedDependencies::Get<GameManager>()->TriggerEvent(GameManager::GameEvent::PRESSED_PAUSE);
}

void GameplayEvents::OnPressedUnpause()
{
	GameSharedDependencies::Get<GameManager>()->TriggerEvent(GameManager::GameEvent::PRESSED_UNPAUSE);
}

void GameplayEvents::OnPressedStartGame()
{
	GameSharedDependencies::Get<GameManager>()->TriggerEvent(GameManager::GameEvent::PRESSED_START_GAME);
}

void GameplayEvents::OnPlayerWon()
{
	GameSharedDependencies::Get<GameManager>()->TriggerEvent(GameManager::GameEvent::PLAYER_WON);
}
