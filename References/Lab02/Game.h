#pragma once

#include "SDL2/SDL.h"
#include "Math.h"
#include <vector>
#include <string>
#include <unordered_map>

class Game
{
public:
	Game() {}
	bool Initialize();
	void Shutdown();
	void RunLoop();

	//Standard Game Constants
	static const int WINDOW_WIDTH = 1024;
	static const int WINDOW_HEIGHT = 786;
	const Vector2 WINDOW_MIN_VECTOR = Vector2(0, 0);
	const Vector2 WINDOW_MAX_VECTOR = Vector2(WINDOW_WIDTH, WINDOW_HEIGHT);

	//Game Logic Constants
	static const int MINIMUM_DELTA_TIME = 16;

	//Add, Remove, and Get Functions
	void AddActor(class Actor*);
	bool RemoveActor(class Actor*);

	void AddAsteroid(class Asteroid*);
	void RemoveAsteroid(class Asteroid*);

	void AddSprite(class SpriteComponent*);
	bool RemoveSprite(class SpriteComponent*);

	SDL_Texture* GetTexture(std::string);
	const std::vector<class Asteroid*> GetAsteroids() { return mAsteroids; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	//Standard Game Variables
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;

	bool mHasQuit = false;
	Uint32 mPreviousTime = 0;

	//Graphics Variables
	std::vector<class Actor*> mActors;
	std::unordered_map<std::string, SDL_Texture*> mTextures;
	std::vector<class SpriteComponent*> mSprites;

	std::vector<class Asteroid*> mAsteroids{};

	void LoadData();
	void UnloadData();
};