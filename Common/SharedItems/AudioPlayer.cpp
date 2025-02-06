#include "AudioPlayer.h"

#include "Engine.h"
#include "UknittySettings.h"
#include <Audio/Sound.hpp>
#include <stdexcept>
#include <string>

namespace Uknitty
{

AudioPlayer::~AudioPlayer()
{
	for(auto& [audioType, soundInstances] : m_soundInstances)
	{
		for(auto& sound : soundInstances)
		{
			delete sound;
		}
		soundInstances.clear();
	}
	m_soundInstances.clear();
}

void AudioPlayer::ReplayOverlapped(Uknitty::AudioType audioType)
{
	CheckExistance(audioType);
	for(auto& sound : m_soundInstances[audioType])
	{
		if(!sound->isPlaying())
		{
			SetVolume(sound, audioType);
			sound->replay();
			return;
		}
	}
	Audio::Sound* copy = CreateCopy(audioType);
	SetVolume(copy, audioType);
	copy->replay();
}

void AudioPlayer::Play(AudioType audioType)
{
	CheckExistance(audioType);
	SetVolume(m_soundInstances[audioType][0], audioType);
	m_soundInstances[audioType][0]->play();
}

void AudioPlayer::Replay(AudioType audioType)
{
	CheckExistance(audioType);
	SetVolume(m_soundInstances[audioType][0], audioType);
	m_soundInstances[audioType][0]->replay();
}

void AudioPlayer::Stop(AudioType audioType)
{
	CheckExistance(audioType);
	m_soundInstances[audioType][0]->stop();
}

void AudioPlayer::SetVolume(AudioType audioType, float volume)
{
	CheckExistance(audioType);
	m_volumes[audioType] = volume;
}

void AudioPlayer::CreateSound(Uknitty::AudioType audioType, std::string filePath, Audio::Sound::Type soundType)
{
	Audio::Sound* sound = new Audio::Sound(filePath, soundType);
	if(soundType != Audio::Sound::Type::Sound)
	{
		sound->setLooping(true);
	}
	m_soundInstances[audioType].push_back(sound);
}

Audio::Sound* AudioPlayer::CreateCopy(Uknitty::AudioType audioType)
{
	Audio::Sound* originalInstance = m_soundInstances[audioType][0];
	Audio::Sound* copyInstance = new Audio::Sound(*originalInstance);
	m_soundInstances[audioType].push_back(copyInstance);
	return copyInstance;
}

void AudioPlayer::CheckExistance(AudioType audioType)
{
	if(m_soundInstances.find(audioType) == m_soundInstances.end())
	{
		throw std::runtime_error("no sound instance of audioType has been created");
	}
}

void Uknitty::AudioPlayer::SetVolume(Audio::Sound* sound, AudioType audioType)
{
	sound->setVolume(m_volumes.find(audioType) != m_volumes.end() ? m_volumes[audioType] : 1.0f);
}

} // namespace Uknitty
