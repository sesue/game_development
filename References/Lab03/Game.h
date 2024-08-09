#pragma once

#include "SDL2/SDL.h"
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
	static const int WINDOW_WIDTH = 448;
	static const int WINDOW_HEIGHT = 512;
	const Vector2 WINDOW_MIN_VECTOR = Vector2(0, 0);
	const Vector2 WINDOW_MAX_VECTOR = Vector2(WINDOW_WIDTH, WINDOW_HEIGHT);

	//Game Graphic Constants
	const int LEVEL_GRID_WIDTH = 32;
	const int LEVEL_GRID_HEIGHT = 32;
	const Vector2 LEVEL_TOP_LEFT = Vector2(32, 80);
	const Vector2 GRID_STEP = Vector2(32, 32);

	//Game Logic Constants
	static const int MINIMUM_DELTA_TIME = 16;

	//Add, Remove, and Get Functions
	void AddActor(class Actor*);
	bool RemoveActor(class Actor*);

	void AddSprite(class SpriteComponent*);
	bool RemoveSprite(class SpriteComponent*);

	void AddVehicle(class Vehicle*);
	bool RemoveVehicle(class Vehicle*);

	void AddLog(class Log*);
	bool RemoveLog(class Log*);

	SDL_Texture* GetTexture(std::string);
	std::vector<class Vehicle*>& GetVehicles() { return mVehicles; }
	std::vector<class Log*>& GetLogs() { return mLogs; }
	class Frog* GetFrog() { return mFrog; }
	class Actor* GetGoal() { return mGoal; }
	class CollisionComponent* GetGoalCollision() { return mGoalCollision; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	//Standard Game Variables
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;

	bool mHasQuit = false;
	Uint32 mPreviousTime = 0;

	std::vector<class Vehicle*> mVehicles;
	std::vector<class Log*> mLogs;

	//Graphics Variables
	std::vector<class Actor*> mActors;
	std::unordered_map<std::string, SDL_Texture*> mTextures;
	std::vector<class SpriteComponent*> mSprites;
	class Frog* mFrog = nullptr;
	class Actor* mGoal = nullptr;
	class CollisionComponent* mGoalCollision = nullptr;

	void LoadData();
	void UnloadData();
};