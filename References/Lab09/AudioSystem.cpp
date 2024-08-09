#include "AudioSystem.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <filesystem>

SoundHandle SoundHandle::Invalid;

// Create the AudioSystem with specified number of channels
// (Defaults to 8 channels)
AudioSystem::AudioSystem(int numChannels)
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_AllocateChannels(numChannels);
	mChannels.resize(numChannels);
}

// Destroy the AudioSystem
AudioSystem::~AudioSystem()
{
	for (auto sound : mSounds)
	{
		Mix_FreeChunk(sound.second);
	}
	mSounds.clear();
	Mix_CloseAudio();
}

// Updates the status of all the active sounds every frame
void AudioSystem::Update(float deltaTime)
{
	for (int i = 0; i < mChannels.size(); i++)
	{
		if (mChannels[i].IsValid())
		{
			if (!Mix_Playing(i))
			{
				mHandleMap.erase(mChannels[i]);
				mChannels[i].Reset();
			}
		}
	}
}

// Plays the sound with the specified name and loops if looping is true
// Returns the SoundHandle which is used to perform any other actions on the
// sound when active
// NOTE: The soundName is without the "Assets/Sounds/" part of the file
//       For example, pass in "ChompLoop.wav" rather than
//       "Assets/Sounds/ChompLoop.wav".
SoundHandle AudioSystem::PlaySound(const std::string& soundName, bool looping, int fadeTimeMS)
{

	Mix_Chunk* sound = GetSound(soundName);

	if (sound == nullptr)
	{
		SDL_Log("[AudioSystem] PlaySound couldn't find sound for %s", soundName.c_str());
		return SoundHandle::Invalid;
	}

	int index = -1;
	SoundHandle oldSound = SoundHandle::Invalid;
	//Check for free channel
	for (int i = 0; i < mChannels.size(); i++)
	{
		if (!mChannels[i].IsValid())
		{
			index = i;
			break;
		}
	}

	//Check for oldest same sound
	if (index == -1 && oldSound == SoundHandle::Invalid)
	{
		for (auto iter = mHandleMap.begin(); iter != mHandleMap.end(); iter++)
		{
			if (iter->second.mSoundName == soundName)
			{
				oldSound = iter->first;
				break;
			}
		}
	}

	//Check for oldest non-looping sound
	if (index == -1 && oldSound == SoundHandle::Invalid)
	{
		for (auto iter = mHandleMap.begin(); iter != mHandleMap.end(); iter++)
		{
			if (!iter->second.mIsLooping)
			{
				oldSound = iter->first;
				break;
			}
		}
	}

	//Check for oldest sound
	if (index == -1 && oldSound == SoundHandle::Invalid)
		oldSound = mHandleMap.begin()->first;

	if (oldSound != SoundHandle::Invalid)
	{
		index = mHandleMap[oldSound].mChannel;
		SDL_Log("[AudioSystem] PlaySound ran out of channels playing %s! Stopping %s",
				soundName.c_str(), mHandleMap[oldSound].mSoundName.c_str());
		mHandleMap.erase(oldSound);
	}

	SoundHandle newSoundHandle = ++mLastHandle;

	HandleInfo newHandleInfo;
	newHandleInfo.mSoundName = soundName;
	newHandleInfo.mIsLooping = looping;
	newHandleInfo.mIsPaused = false;
	newHandleInfo.mChannel = index;

	mHandleMap[newSoundHandle] = newHandleInfo;
	mChannels[index] = newSoundHandle;

	if (fadeTimeMS > 0)
	{
		if (looping)
			Mix_FadeInChannel(index, sound, -1, fadeTimeMS);
		else
			Mix_FadeInChannel(index, sound, 0, fadeTimeMS);
	}
	else
	{
		if (looping)
			Mix_PlayChannel(index, sound, -1);
		else
			Mix_PlayChannel(index, sound, 0);
	}

	return newSoundHandle;
}

// Stops the sound if it is currently playing
void AudioSystem::StopSound(SoundHandle sound, int fadeTimeMS)
{
	if (mHandleMap.find(sound) == mHandleMap.end())
	{
		SDL_Log("[AudioSystem] StopSound couldn't find handle %s", sound.GetDebugStr());
		return;
	}
	else
	{
		if (fadeTimeMS > 0)
		{
			Mix_FadeOutChannel(mHandleMap[sound].mChannel, fadeTimeMS);
		}
		else
		{
			Mix_HaltChannel(mHandleMap[sound].mChannel);
			mChannels[mHandleMap[sound].mChannel].Reset();
			mHandleMap.erase(sound);
		}
	}
}

// Pauses the sound if it is currently playing
void AudioSystem::PauseSound(SoundHandle sound)
{
	if (mHandleMap.find(sound) == mHandleMap.end())
	{
		SDL_Log("[AudioSystem] PauseSound couldn't find handle %s", sound.GetDebugStr());
		return;
	}

	if (!mHandleMap[sound].mIsPaused)
	{
		Mix_Pause(mHandleMap[sound].mChannel);
		mHandleMap[sound].mIsPaused = true;
	}
}

// Resumes the sound if it is currently paused
void AudioSystem::ResumeSound(SoundHandle sound)
{
	if (mHandleMap.find(sound) == mHandleMap.end())
	{
		SDL_Log("[AudioSystem] ResumeSound couldn't find handle %s", sound.GetDebugStr());
		return;
	}

	if (mHandleMap[sound].mIsPaused)
	{
		Mix_Resume(mHandleMap[sound].mChannel);
		mHandleMap[sound].mIsPaused = false;
	}
}

// Returns the current state of the sound
SoundState AudioSystem::GetSoundState(SoundHandle sound)
{
	if (mHandleMap.find(sound) == mHandleMap.end())
		return SoundState::Stopped;
	else
	{
		if (mHandleMap[sound].mIsPaused)
			return SoundState::Paused;
		else
			return SoundState::Playing;
	}
}

// Stops all sounds on all channels
void AudioSystem::StopAllSounds()
{
	Mix_HaltChannel(-1);
	for (SoundHandle sound : mChannels)
		sound.Reset();

	mHandleMap.clear();
}

// Cache all sounds under Assets/Sounds
void AudioSystem::CacheAllSounds()
{
#ifndef __clang_analyzer__
	std::error_code ec{};
	for (const auto& rootDirEntry : std::filesystem::directory_iterator{"Assets/Sounds", ec})
	{
		std::string extension = rootDirEntry.path().extension().string();
		if (extension == ".ogg" || extension == ".wav")
		{
			std::string fileName = rootDirEntry.path().stem().string();
			fileName += extension;
			CacheSound(fileName);
		}
	}
#endif
}

// Used to preload the sound data of a sound
// NOTE: The soundName is without the "Assets/Sounds/" part of the file
//       For example, pass in "ChompLoop.wav" rather than
//       "Assets/Sounds/ChompLoop.wav".
void AudioSystem::CacheSound(const std::string& soundName)
{
	GetSound(soundName);
}

// If the sound is already loaded, returns Mix_Chunk from the map.
// Otherwise, will attempt to load the file and save it in the map.
// Returns nullptr if sound is not found.
// NOTE: The soundName is without the "Assets/Sounds/" part of the file
//       For example, pass in "ChompLoop.wav" rather than
//       "Assets/Sounds/ChompLoop.wav".
Mix_Chunk* AudioSystem::GetSound(const std::string& soundName)
{
	std::string fileName = "Assets/Sounds/";
	fileName += soundName;

	Mix_Chunk* chunk = nullptr;
	auto iter = mSounds.find(fileName);
	if (iter != mSounds.end())
	{
		chunk = iter->second;
	}
	else
	{
		chunk = Mix_LoadWAV(fileName.c_str());
		if (!chunk)
		{
			SDL_Log("[AudioSystem] Failed to load sound file %s", fileName.c_str());
			return nullptr;
		}

		mSounds.emplace(fileName, chunk);
	}
	return chunk;
}

// Input for debugging purposes
void AudioSystem::ProcessInput(const Uint8* keyState)
{
	// Debugging code that outputs all active sounds on leading edge of period key
	if (keyState[SDL_SCANCODE_PERIOD] && !mLastDebugKey)
	{
		SDL_Log("[AudioSystem] Active Sounds:");
		for (size_t i = 0; i < mChannels.size(); i++)
		{
			if (mChannels[i].IsValid())
			{
				auto iter = mHandleMap.find(mChannels[i]);
				if (iter != mHandleMap.end())
				{
					HandleInfo& hi = iter->second;
					SDL_Log("Channel %d: %s, %s, looping = %d, paused = %d",
							static_cast<unsigned>(i), mChannels[i].GetDebugStr(),
							hi.mSoundName.c_str(), hi.mIsLooping, hi.mIsPaused);
				}
				else
				{
					SDL_Log("Channel %d: %s INVALID", static_cast<unsigned>(i),
							mChannels[i].GetDebugStr());
				}
			}
		}
	}

	mLastDebugKey = keyState[SDL_SCANCODE_PERIOD];
}
