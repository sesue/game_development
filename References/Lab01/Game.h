#pragma once

#include "SDL2/SDL.h"

class Game
{
public:
	Game() {}
	bool Initialize();
	const void Shutdown();
	void RunLoop();

	//Window Constants
	static const int WINDOW_WIDTH = 1024;
	static const int WINDOW_HEIGHT = 786;

	//Game Graphic Constants
	static const int WALL_THICKNESS = 10;
	static const int PADDLE_HEIGHT = WINDOW_WIDTH / 9 + 1;
	static const int PADDLE_EDGE_BUFFER = 5;
	static const int BALL_HEIGHT = 10;

	//Game Logic Constants
	static const int MINIMUM_DELTA_TIME = 16;
	static const int BOUNCE_BORDER = WALL_THICKNESS + (BALL_HEIGHT / 2);
	static const int BOUNCE_BUFFER = 3;

private:
	void ProcessInput();
	void UpdateGame();
	const void GenerateOutput();

	//Window Variables
	SDL_Window* mWindow = nullptr;
	SDL_Renderer* mRenderer = nullptr;

	//Game Graphics Variables
	SDL_Point mPaddlePosition{WALL_THICKNESS / 2 + PADDLE_EDGE_BUFFER, WINDOW_HEIGHT / 2};
	SDL_Point mBallPosition{WINDOW_HEIGHT / 2, WINDOW_HEIGHT / 2};

	//Game Logic Variables
	bool mHasQuit = false;
	Uint32 mPreviousTime = 0;
	int mPaddleSpeed = WINDOW_HEIGHT / 3;
	int mPaddleMovement = 0;
	SDL_Point mBallSpeed{WINDOW_HEIGHT / 3, WINDOW_HEIGHT / 3};
};