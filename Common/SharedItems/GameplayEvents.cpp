#include "GameplayEvents.h"

#include "AudioManager.h"
#include "GameManager.h"
#include "GameSharedDependencies.h"
#include "Player.h"
#include "UIManager.h"

GameplayEvents::GameplayEvents()
{
	GameSharedDependencies::Set<GameplayEvents>(this);
}

void GameplayEvents::OnEnemyBulletHitPlayer()
{
	GameSharedDependencies::Get<Player>()->OnEnemyBulletHit();
	GameSharedDependencies::Get<UIManager>()->PlayRedScreenEffect();
	GameSharedDependencies::Get<AudioManager>()->OnPlayerHurt();
}

void GameplayEvents::OnPlayerBulletHitEnemy()
{
	GameSharedDependencies::Get<UIManager>()->PlayHitMarkerEffect();
	GameSharedDependencies::Get<AudioManager>()->OnEnemyHurt();
}

void GameplayEvents::OnPlayerShotGun()
{
	GameSharedDependencies::Get<AudioManager>()->OnPlayerShotGun();
}

void GameplayEvents::OnEnemyShotGun()
{
	GameSharedDependencies::Get<AudioManager>()->OnEnemyShotGun();
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

void GameplayEvents::OnPlayerCollidedWithRoomChange()
{
	GameSharedDependencies::Get<GameManager>()->TriggerEvent(GameManager::GameEvent::PLAYER_COLLIDED_WITH_ROOM_CHANGE);
}

void GameplayEvents::OnNewSceneLoaded()
{
	GameSharedDependencies::Get<GameManager>()->TriggerEvent(GameManager::GameEvent::NEW_SCENE_LOADED);
}

void GameplayEvents::OnSelectedInventoryItem()
{
	GameSharedDependencies::Get<GameManager>()->TriggerEvent(GameManager::GameEvent::PRESSED_INVENTORY);
}

void GameplayEvents::OnUsedHamburger()
{
	GameSharedDependencies::Get<Player>()->OnUsedHamburger();
}

void GameplayEvents::OnUsedGun()
{
	GameSharedDependencies::Get<Player>()->OnUsedGun();
}

void GameplayEvents::OnCameraFolowTypeChanged()
{
	GameSharedDependencies::Get<Player>()->OnCameraFolowTypeChanged();
}

void GameplayEvents::OnOneEnemyAlarmed()
{
	GameSharedDependencies::Get<GameManager>()->TriggerEvent(GameManager::GameEvent::ONE_ENEMY_ALARMED);
}
