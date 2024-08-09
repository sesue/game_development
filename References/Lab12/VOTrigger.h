#pragma once
#include "Actor.h"
#include <vector>
#include <string>

#include "AudioSystem.h"

class VOTrigger : public Actor
{
public:
	VOTrigger(class Game* game, std::string door = "", std::string nextLevel = "",
			  std::vector<std::string> sounds = {}, std::vector<std::string> subtitles = {});
	~VOTrigger();

	void OnUpdate(float deltaTime) override;
	void OnProcessInput(const Uint8* keyState, Uint32 mouseButtons, const Vector2& relativeMouse) override;

	const Vector3 COLLISION_SIZE = Vector3(1.0f, 1.0f, 1.0f);

private:
	std::string mDoorName;
	std::string mNextLevel;
	std::vector<std::string> mSounds;
	std::vector<std::string> mSubtitles;

	void NextAction(class AudioSystem* audio);
	int mNextSound = -1;
	SoundHandle mCurrSound = SoundHandle::Invalid;
	bool mPrevFrameF = false;
};