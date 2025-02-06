#pragma once

#include "UknittySettings.h"
#include <array>

namespace Uknitty
{
class AudioPlayer;
}

class AudioManager
{
public:
	AudioManager();

	void OnPlayerHurt();
	void OnEnemyHurt();
	void OnPlayerShotGun();
	void OnEnemyShotGun();
	void OnLevelAlert();
	void OnLevelNormal();
	void OnStartGame();
	void OnPause();
	void OnResume();
	void OnMainMenu();


private:
	Uknitty::AudioPlayer* m_audioPlayer = nullptr;

	const std::array<Uknitty::AudioType, 2> ENEMY_HURT_TYPES = {Uknitty::AudioType::EnemyHurt0, Uknitty::AudioType::EnemyHurt1};
	const float BGM_ALERT_PITCH = 3.0f;

	int m_lastEnemyHurtAudioIndex = 0;
	Uknitty::AudioType m_lastBGMPlaying = Uknitty::AudioType::BGM_Slow;
};
