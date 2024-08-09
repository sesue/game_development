//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"

bool Game::Initialize()
{
	//Setup SDL, Window, and Renderer for Game
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO))
	{
		return false;
	}
	mWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							   Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, 0);
	if (!mWindow)
	{
		return false;
	}
	mRenderer = SDL_CreateRenderer(mWindow, -1,
								   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	return mRenderer;
}

const void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::RunLoop()
{
	while (!mHasQuit)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mHasQuit = true;
			break;
		}
	}

	mPaddleMovement = 0;

	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_ESCAPE])
		mHasQuit = true;
	if (keyState[SDL_SCANCODE_W])
		mPaddleMovement += 1;
	if (keyState[SDL_SCANCODE_S])
		mPaddleMovement -= 1;
}

void Game::UpdateGame()
{
	//Frame Limit
	Uint32 currentTime = SDL_GetTicks();
	while ((currentTime - mPreviousTime) < MINIMUM_DELTA_TIME)
	{
		currentTime = SDL_GetTicks();
	}

	//Calculate Delta Time (seconds as float)
	float deltaTime = static_cast<float>(currentTime - mPreviousTime) / 1000.0f;
	mPreviousTime = currentTime;

	//Cap to maximum delta time
	if (deltaTime > 0.033f)
	{
		deltaTime = 0.033f;
	}

	//Update all game objects by delta time
	if (mPaddleMovement == -1 &&
		(mPaddlePosition.y < (WINDOW_HEIGHT - WALL_THICKNESS - (PADDLE_HEIGHT / 2))))
	{
		mPaddlePosition.y += static_cast<int>(mPaddleSpeed * deltaTime);
	}

	if (mPaddleMovement == 1 && (mPaddlePosition.y > (WALL_THICKNESS + (PADDLE_HEIGHT / 2))))
	{
		mPaddlePosition.y -= static_cast<int>(mPaddleSpeed * deltaTime);
	}

	//Ball Movement
	mBallPosition.x += static_cast<int>(mBallSpeed.x * deltaTime);
	mBallPosition.y += static_cast<int>(mBallSpeed.y * deltaTime);

	//Bottom Wall Bounce
	if (mBallPosition.y > (WINDOW_HEIGHT - BOUNCE_BORDER))
	{
		mBallSpeed.y = -mBallSpeed.y;
		mBallPosition.y = WINDOW_HEIGHT - BOUNCE_BORDER - 1;
	}

	//Top Wall Bounce
	if (mBallPosition.y < BOUNCE_BORDER)
	{
		mBallSpeed.y = -mBallSpeed.y;
		mBallPosition.y = BOUNCE_BORDER + 1;
	}

	//Right Wall Bounce
	if (mBallPosition.x > (WINDOW_WIDTH - BOUNCE_BORDER))
	{
		mBallSpeed.x = -mBallSpeed.x;
		mBallPosition.x = WINDOW_WIDTH - BOUNCE_BORDER - 1;
	}

	//Paddle Bounce
	if ((mBallPosition.x < BOUNCE_BORDER + PADDLE_EDGE_BUFFER) &&
		(mBallPosition.y < mPaddlePosition.y + (PADDLE_HEIGHT / 2) + (BALL_HEIGHT / 2)) &&
		(mBallPosition.y > mPaddlePosition.y - (PADDLE_HEIGHT / 2) - (BALL_HEIGHT / 2)) &&
		(mBallPosition.x > BOUNCE_BORDER - BOUNCE_BUFFER))
	{
		mBallSpeed.x = -mBallSpeed.x;
		mBallPosition.x = BOUNCE_BORDER + PADDLE_EDGE_BUFFER + 1;
	}

	//End Game
	if (mBallPosition.x < 0 - BALL_HEIGHT)
		mHasQuit = true;
}

SDL_Rect BuildSDLRect(int x, int y, int w, int h)
{
	SDL_Rect rectangle;
	rectangle.x = x;
	rectangle.y = y;
	rectangle.w = w;
	rectangle.h = h;
	return rectangle;
}

const void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	//Draw Game Walls
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	SDL_Rect topWall = BuildSDLRect(0, 0, WINDOW_WIDTH, WALL_THICKNESS);
	SDL_Rect rightWall = BuildSDLRect(WINDOW_WIDTH - WALL_THICKNESS, 0, WALL_THICKNESS,
									  WINDOW_HEIGHT);
	SDL_Rect botWall = BuildSDLRect(0, WINDOW_HEIGHT - WALL_THICKNESS, WINDOW_WIDTH,
									WALL_THICKNESS);
	SDL_RenderFillRect(mRenderer, &topWall);
	SDL_RenderFillRect(mRenderer, &rightWall);
	SDL_RenderFillRect(mRenderer, &botWall);

	//Draw Game Paddle
	SDL_Rect paddle = BuildSDLRect(mPaddlePosition.x - (WALL_THICKNESS / 2),
								   mPaddlePosition.y - (PADDLE_HEIGHT / 2), WALL_THICKNESS,
								   PADDLE_HEIGHT);
	SDL_RenderFillRect(mRenderer, &paddle);

	//Draw Game Ball
	SDL_Rect ball = BuildSDLRect(mBallPosition.x - (BALL_HEIGHT / 2),
								 mBallPosition.y - (BALL_HEIGHT / 2), BALL_HEIGHT, BALL_HEIGHT);
	SDL_RenderFillRect(mRenderer, &ball);

	SDL_RenderPresent(mRenderer);
}