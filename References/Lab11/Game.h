#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include "AudioSystem.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define emscripten_cancel_main_loop()
#endif

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void EmRunIteration()
	{
		if (!mIsRunning)
		{
			emscripten_cancel_main_loop();
		}
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddCollider(class Actor* collider);
	void RemoveCollider(class Actor* collider);

	void AddDoor(class Door* door);
	class Door* GetDoor(std::string name) { return mDoors[name]; }

	class Renderer* GetRenderer() { return mRenderer; }

	class Player* GetPlayer() { return mPlayer; }
	void SetPlayer(class Player* player) { mPlayer = player; }

	const std::vector<class Actor*>& GetActors() { return mActors; }
	std::vector<class Actor*>& GetColliders() { return mColliders; }

	void SetBluePortal(class Portal* portal) { mBluePortal = portal; }
	void SetOrangePortal(class Portal* portal) { mOrangePortal = portal; }
	class Portal* GetBluePortal() { return mBluePortal; }
	class Portal* GetOrangePortal() { return mOrangePortal; }

	void ReloadLevel() { mLoadLevel = mCurrentLevel; }

	AudioSystem* GetAudio() { return mAudio; }
	void SetSoundHandle(SoundHandle sh) { mMusic = sh; }
	const SoundHandle GetSoundHandle() const { return mMusic; }

	const float WINDOW_WIDTH = 1024.0f;
	const float WINDOW_HEIGHT = 768.0f;

	const std::string LEVEL_FILE = "Assets/Lab09.json";

	std::string mCurrentLevel;
	class InputReplay* mInputReplay = nullptr;

	Quaternion CreateArbitraryQuaternion(Vector3 originalFacing, Vector3 desiredFacing);

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// Input Variables
	std::map<std::string, bool> mPreviousFrameKeyboard = {{"SDL_SCANCODE_F5", false}};

	// All the actors in the game
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mColliders;

	class Player* mPlayer = nullptr;

	class Renderer* mRenderer = nullptr;
	AudioSystem* mAudio = nullptr;
	SoundHandle mMusic = SoundHandle::Invalid;
	std::string mLoadLevel;

	Uint32 mTicksCount = 0;
	bool mIsRunning = true;

	//Portal Variables
	class Portal* mBluePortal = nullptr;
	class Portal* mOrangePortal = nullptr;

	//Door Variables
	std::map<std::string, class Door*> mDoors;
};
