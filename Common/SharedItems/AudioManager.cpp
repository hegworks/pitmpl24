#include "AudioManager.h"

#include "AudioPlayer.h"
#include "Engine.h"
#include "GameSharedDependencies.h"
#include "UknittySettings.h"

AudioManager::AudioManager()
{
	GameSharedDependencies::Set<AudioManager>(this);

	m_audioPlayer = Uknitty::Engine::GetInstance()->GetAudioPlayer();
	m_audioPlayer->CreateSound(Uknitty::AudioType::BGM_Slow, "../Common/Assets/Audio/BGM_Slow.ogg", Audio::Sound::Type::Music);
	m_audioPlayer->CreateSound(Uknitty::AudioType::BGM_Fast, "../Common/Assets/Audio/BGM_Fast.ogg", Audio::Sound::Type::Music);
	m_audioPlayer->CreateSound(Uknitty::AudioType::PlayerGunShot, "../Common/Assets/Audio/PlayerGunShot.ogg", Audio::Sound::Type::Sound);
	m_audioPlayer->CreateSound(Uknitty::AudioType::EnemyGunShot, "../Common/Assets/Audio/EnemyGunShot.ogg", Audio::Sound::Type::Sound);
	m_audioPlayer->CreateSound(Uknitty::AudioType::PlayerHurt, "../Common/Assets/Audio/PlayerHurt.ogg", Audio::Sound::Type::Sound);
	m_audioPlayer->CreateSound(Uknitty::AudioType::EnemyHurt0, "../Common/Assets/Audio/EnemyHurt0.ogg", Audio::Sound::Type::Sound);
	m_audioPlayer->CreateSound(Uknitty::AudioType::EnemyHurt1, "../Common/Assets/Audio/EnemyHurt1.ogg", Audio::Sound::Type::Sound);

	m_audioPlayer->SetVolume(Uknitty::AudioType::PlayerHurt, 0.5f);
	m_audioPlayer->SetVolume(Uknitty::AudioType::BGM_Slow, 0.25f);
	m_audioPlayer->SetVolume(Uknitty::AudioType::BGM_Fast, 0.25f);
}

void AudioManager::OnPlayerHurt()
{
	m_audioPlayer->ReplayOverlapped(Uknitty::AudioType::PlayerHurt);
}

void AudioManager::OnEnemyHurt()
{
	m_lastEnemyHurtAudioIndex = (m_lastEnemyHurtAudioIndex + 1) % ENEMY_HURT_TYPES.size();
	m_audioPlayer->ReplayOverlapped(ENEMY_HURT_TYPES[m_lastEnemyHurtAudioIndex]);
}

void AudioManager::OnPlayerShotGun()
{
	m_audioPlayer->ReplayOverlapped(Uknitty::AudioType::PlayerGunShot);
}

void AudioManager::OnEnemyShotGun()
{
	m_audioPlayer->ReplayOverlapped(Uknitty::AudioType::EnemyGunShot);
}

void AudioManager::OnLevelAlert()
{
	m_audioPlayer->SetPitch(Uknitty::AudioType::BGM_Slow, BGM_ALERT_PITCH);
}

void AudioManager::OnLevelNormal()
{
	m_audioPlayer->SetPitch(Uknitty::AudioType::BGM_Slow, 1.0f);
}

void AudioManager::OnStartGame()
{
	m_audioPlayer->SetPitch(Uknitty::AudioType::BGM_Slow, 1.0f);
	m_audioPlayer->Play(Uknitty::AudioType::BGM_Slow);
}

void AudioManager::OnPause()
{
	m_audioPlayer->Stop(Uknitty::AudioType::BGM_Slow);
}

void AudioManager::OnResume()
{
	m_audioPlayer->Play(Uknitty::AudioType::BGM_Slow);
}

void AudioManager::OnMainMenu()
{
	m_audioPlayer->Seek(Uknitty::AudioType::BGM_Slow, 0);
	m_audioPlayer->Stop(Uknitty::AudioType::BGM_Slow);
}
