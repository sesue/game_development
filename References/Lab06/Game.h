#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "AudioSystem.h"
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
	static const int WINDOW_WIDTH = 512;
	static const int WINDOW_HEIGHT = 448;
	const Vector2 WINDOW_MIN_VECTOR = Vector2(0, 0);
	const Vector2 WINDOW_MAX_VECTOR = Vector2(WINDOW_WIDTH, WINDOW_HEIGHT);

	//Game Graphic Constants
	const std::string BACKGROUND_FILE_PATH = "Assets/Map/Tiles.csv";
	const std::string TEXTURE_FILE_PATH = "Assets/Map/Tiles.png";
	const std::string OBJECT_FILE_PATH = "Assets/Map/Objects.csv";
	const int TILE_WIDTH = 32;
	const int TILE_HEIGHT = 32;
	const Vector2 LEVEL_TOP_LEFT = Vector2(16, 16);
	const Vector2 GRID_STEP = Vector2(32, 32);

	//Game Logic Constants
	static const int MINIMUM_DELTA_TIME = 16;

	//Add, Remove, and Get Functions
	void AddActor(class Actor*);
	bool RemoveActor(class Actor*);

	void AddSprite(class SpriteComponent*);
	bool RemoveSprite(class SpriteComponent*);

	void AddCollider(class Collider*);
	bool RemoveCollider(class Collider*);

	void AddEnemyComponent(class EnemyComponent*);
	bool RemoveEnemyComponent(class EnemyComponent*);

	SDL_Texture* GetTexture(std::string);

	const class Player* GetPlayer() { return mPlayer; }
	const std::vector<class Collider*>& GetColliders() const { return mColliders; }
	const std::vector<class EnemyComponent*>& GetEnemies() const { return mEnemies; }
	class PathFinder* GetPaths() { return mPathFinder; }
	Vector2& GetCameraPos() { return mCameraPosition; }
	class AudioSystem* GetAudio() { return mAudioSystem; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	void LoadData();
	void LoadObjects();
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
	std::vector<class SpriteComponent*> mSprites;
	std::vector<class Collider*> mColliders;
	std::vector<class EnemyComponent*> mEnemies;

	class PathFinder* mPathFinder = nullptr;
	class Player* mPlayer = nullptr;
	AudioSystem* mAudioSystem = nullptr;
	SoundHandle mSoundHandle;
};