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

	void AddBlock(class Block* block);
	void RemoveBlock(class Block* block);

	std::vector<class Block*>& GetBlocks() { return mBlocks; }

	AudioSystem* GetAudio() { return mAudio; }

	class Renderer* GetRenderer() { return mRenderer; }

	class Player* GetPlayer() { return mPlayer; }

	const float WINDOW_WIDTH = 1024.0f;
	const float WINDOW_HEIGHT = 768.0f;

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	void LoadBlocks(std::string file);

	const int MIN_BLOCK_FILE = 1;
	const int MAX_BLOCK_FILE = 20;

	const float BLOCK_Y_START = -237.5f;
	const float BLOCK_Z_START = 237.5f;
	const float BLOCK_Y_INCREMENT = 25.0f;
	const float BLOCK_Z_INCREMENT = -25.0f;
	const float BLOCK_X_INCREMENT = 1000.0f;
	const float BLOCK_LOAD_DISTANCE = 4000.0f;

	float mNextBlockLoadX = BLOCK_X_INCREMENT;

	int mNextBlockFile = MIN_BLOCK_FILE;
	bool mIsRandomBlockFile = false;

	// All the actors in the game
	std::vector<class Actor*> mActors;
	std::vector<class Block*> mBlocks;

	class Player* mPlayer = nullptr;

	class Renderer* mRenderer = nullptr;
	AudioSystem* mAudio = nullptr;

	Uint32 mTicksCount = 0;
	bool mIsRunning = true;
};
