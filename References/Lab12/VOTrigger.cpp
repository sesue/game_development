// Header Include
#include "VOTrigger.h"

// Actor Includes
#include "Player.h"
#include "Door.h"

// Component Includes
#include "CollisionComponent.h"
#include "HealthComponent.h"
#include "HUD.h"

// Utility Include
#include "Game.h"
#include "AudioSystem.h"



VOTrigger::VOTrigger(Game* game, std::string door, std::string nextLevel,
					 std::vector<std::string> sounds, std::vector<std::string> subtitles)
: Actor(game)
, mDoorName(door)
, mNextLevel(nextLevel)
, mSounds(sounds)
, mSubtitles(subtitles)
{
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(COLLISION_SIZE.x, COLLISION_SIZE.y, COLLISION_SIZE.z);
}

VOTrigger::~VOTrigger()
{
}

void VOTrigger::OnUpdate(float deltaTime)
{
	AudioSystem* audio = mGame->GetAudio();

	if (mGame->GetPlayer()->GetComponent<HealthComponent>()->IsDead())
	{
		if (audio->GetSoundState(mCurrSound) == SoundState::Playing)
			audio->StopSound(mCurrSound);

		return;
	}

	if (mNextSound == -1)
	{
		if (GetComponent<CollisionComponent>()->Intersect(
				mGame->GetPlayer()->GetComponent<CollisionComponent>()))
		{
			mNextSound = 0;
			NextAction(audio);
		}
	}
	else if (audio->GetSoundState(mCurrSound) == SoundState::Stopped)
	{
		NextAction(audio);
	}
}

void VOTrigger::OnProcessInput(const Uint8* keyState, Uint32 mouseButtons,
							   const Vector2& relativeMouse)
{
	if (mNextSound > -1)
	{
		if (keyState[SDL_SCANCODE_F] && !mPrevFrameF)
		{
			AudioSystem* audio = mGame->GetAudio();
			if (audio->GetSoundState(mCurrSound) == SoundState::Playing)
			{
				audio->StopSound(mCurrSound);
				NextAction(audio);
			}
		}

		mPrevFrameF = keyState[SDL_SCANCODE_F];
	}
}

void VOTrigger::NextAction(AudioSystem* audio)
{
	if (mNextSound < mSounds.size())
	{
		mCurrSound = audio->PlaySound(mSounds[mNextSound]);
		mGame->GetPlayer()->GetComponent<HUD>()->ShowSubtitle(mSubtitles[mNextSound]);
		mNextSound++;
	}
	else
	{
		mGame->GetPlayer()->GetComponent<HUD>()->ShowSubtitle("");

		if (!mDoorName.empty())
			mGame->GetDoor(mDoorName)->Open();

		if (!mNextLevel.empty())
		{
			mGame->mCurrentLevel = mNextLevel;
			mGame->ReloadLevel();
		}

		mState = ActorState::Destroy;
	}
}