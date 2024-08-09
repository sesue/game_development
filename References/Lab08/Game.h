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

	AudioSystem* GetAudio() { return mAudio; }

	class Renderer* GetRenderer() { return mRenderer; }

	class Player* GetPlayer() { return mPlayer; }
	class Enemy* GetEnemy() { return mEnemy; }

	class HeightMap* GetHeightMap() { return mHeightMap; }

	void SetSoundHandle(SoundHandle sh) { mMusic = sh; }
	const SoundHandle GetSoundHandle() const { return mMusic; }

	const float WINDOW_WIDTH = 1024.0f;
	const float WINDOW_HEIGHT = 768.0f;

	const float START_UP_TIME = 8.5f;

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// All the actors in the game
	std::vector<class Actor*> mActors;

	class Player* mPlayer = nullptr;
	class Enemy* mEnemy = nullptr;

	class Renderer* mRenderer = nullptr;
	AudioSystem* mAudio = nullptr;
	class HeightMap* mHeightMap = nullptr;

	Uint32 mTicksCount = 0;
	bool mIsRunning = true;

	float mStartUpTimer = START_UP_TIME;
	SoundHandle mMusic = SoundHandle::Invalid;
};
