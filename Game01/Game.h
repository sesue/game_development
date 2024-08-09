#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "Math.h"
#include <vector>
#include <string>
#include <map>
#include <unordered_map>

class Game
{
public:
	Game() {}
	bool Initialize();
	void Shutdown();
	void RunLoop();

	//Standard Game Constants
	static const int WINDOW_WIDTH = 600;
	static const int WINDOW_HEIGHT = 448;
	const Vector2 WINDOW_MIN_VECTOR = Vector2(0, 0);
	const Vector2 WINDOW_MAX_VECTOR = Vector2(WINDOW_WIDTH, WINDOW_HEIGHT);

	//Game Graphic Constants
	const int LEVEL_GRID_WIDTH = 32;
	const int LEVEL_GRID_HEIGHT = 32;
	const Vector2 LEVEL_TOP_LEFT = Vector2(16, 16);
	const Vector2 GRID_STEP = Vector2(32, 32);

	//Game Logic Constants
	static const int MINIMUM_DELTA_TIME = 16;
	const float GAME_GRAVITY = 2000.0f;
	const float GAME_FLOOR = static_cast<float>(WINDOW_HEIGHT) +
							 static_cast<float>(LEVEL_GRID_HEIGHT);
	const float FLAG_POLE_X = 6368.0f;

	//Add, Remove, and Get Functions
	void AddActor(class Actor*);
	bool RemoveActor(class Actor*);

	void AddSprite(class SpriteComponent*);
	bool RemoveSprite(class SpriteComponent*);

	void AddBlock(class Block*);
	bool RemoveBlock(class Block*);

	void AddEnemy(class Goomba*);
	bool RemoveEnemy(class Goomba*);

	SDL_Texture* GetTexture(std::string);
	Mix_Chunk* GetSound(const std::string& filename);
	int GetMusicChannel() const { return mMusicChannel; }

	const std::vector<class Block*>& GetBlocks() { return mBlocks; }
	const std::vector<class Goomba*>& GetEnemies() { return mEnemies; }
	const class Player* GetPlayer() { return mPlayer; }
	Vector2& GetCameraPos() { return mCameraPosition; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	void LoadData();
	void UnloadData();

	//Standard Game Variables
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;

	bool mHasQuit = false;
	Uint32 mPreviousTime = 0;

	//Graphics Variables
	Vector2 mBackgroundPosition = Vector2(3392, 224);
	Vector2 mCameraPosition = Vector2(0, 0);

	std::vector<class Actor*> mActors;
	std::unordered_map<std::string, SDL_Texture*> mTextures;
	std::unordered_map<std::string, Mix_Chunk*> mSounds;
	std::vector<class SpriteComponent*> mSprites;

	std::vector<class Block*> mBlocks;
	std::vector<class Goomba*> mEnemies;
	class Player* mPlayer = nullptr;
	int mMusicChannel = 0;
};