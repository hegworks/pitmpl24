#pragma once

#include "UknittySettings.h"
#include <unordered_map>

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
	void OnPause();
	void OnResume();
	void OnMainMenu();

private:
	Uknitty::AudioPlayer* m_audioPlayer = nullptr;

	const int TOTAL_ENEMY_HURT_SOUNDS = 2;
	const float BGM_SLOW_LENGTH = 200;
	const float BGM_FAST_LENGTH = 100;
	const Uknitty::AudioType ENMEY_HURT_TYPES[2] = {Uknitty::AudioType::EnemyHurt0, Uknitty::AudioType::EnemyHurt1};

	int m_lastEnemyHurtAudioIndex = 0;
	Uknitty::AudioType m_lastBGMPlaying = Uknitty::AudioType::BGM_Slow;
};
