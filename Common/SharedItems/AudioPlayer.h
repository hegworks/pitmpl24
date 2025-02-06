#pragma once

#include "Audio/Sound.hpp"
#include "UknittySettings.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace Uknitty
{

class AudioPlayer
{
public:
	AudioPlayer() = default;
	~AudioPlayer();

	/// will create the first instance of a sound
	void CreateSound(Uknitty::AudioType audioType, std::string filePath, Audio::Sound::Type soundType);
	/// will play a non-playing instance of the sound
	void ReplayOverlapped(Uknitty::AudioType audioType);
	/// will play the first created instance of the sound
	void Play(AudioType audioType);
	/// will play the first created instance of the sound from the beginning
	void Replay(AudioType audioType);
	/// will stop/pause the first created instance of the sound
	void Stop(AudioType audioType);
	void SetVolume(AudioType audioType, float volume);

private:
	std::unordered_map<Uknitty::AudioType, std::vector<Audio::Sound*>> m_soundInstances;
	std::unordered_map<Uknitty::AudioType, float> m_volumes;

	Audio::Sound* CreateCopy(Uknitty::AudioType audioType);
	void CheckExistance(AudioType audioType);
	void SetVolume(Audio::Sound* sound, AudioType audioType);
};

} // namespace Uknitty
