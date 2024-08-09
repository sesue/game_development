//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "Actor.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Laser.h"
#include "SpriteComponent.h"
#include "Random.h"
#include <algorithm>
#include <SDL2/SDL_image.h>

bool Game::Initialize()
{
	Random::Init();

	//Setup SDL, Window, and Renderer for Game
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO))
	{
		return false;
	}
	mWindow = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							   Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, 0);
	if (mWindow == nullptr)
	{
		SDL_Quit();
		return false;
	}
	mRenderer = SDL_CreateRenderer(mWindow, -1,
								   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (mRenderer == nullptr)
	{
		SDL_DestroyWindow(mWindow);
		SDL_Quit();
		return false;
	}

	IMG_Init(IMG_INIT_PNG);

	LoadData();

	return true;
}

void Game::Shutdown()
{
	UnloadData();

	IMG_Quit();
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

	const Uint8* keyState = SDL_GetKeyboardState(NULL);

	for (Actor* actorPtr : mActors)
	{
		actorPtr->ProcessInput(keyState);
	}

	if (keyState[SDL_SCANCODE_ESCAPE])
		mHasQuit = true;
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
	std::vector<Actor*> copyActors = mActors;
	for (Actor* actorPtr : copyActors)
	{
		actorPtr->Update(deltaTime);
	}

	std::vector<Actor*> actorsToDestory;
	for (Actor* actorPtr : mActors)
	{
		if (actorPtr->GetState() == ActorState::Destroy)
		{
			actorsToDestory.push_back(actorPtr);
		}
	}

	for (Actor* actorPtr : actorsToDestory)
	{
		delete actorPtr;
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	//Draw
	for (SpriteComponent* sprite : mSprites)
	{
		if (sprite->IsVisible())
		{
			sprite->Draw(mRenderer);
		}
	}

	SDL_RenderPresent(mRenderer);
}

void Game::AddActor(Actor* actor)
{
	mActors.push_back(actor);
}

bool Game::RemoveActor(Actor* actor)
{
	auto actorPtr = std::find(mActors.begin(), mActors.end(), actor);

	if (actorPtr == mActors.end())
	{
		return false;
	}
	else
	{
		mActors.erase(actorPtr);
		return true;
	}
}

void Game::AddSprite(SpriteComponent* sprite)
{
	mSprites.push_back(sprite);

	std::sort(mSprites.begin(), mSprites.end(), [](SpriteComponent* a, SpriteComponent* b) {
		return a->GetDrawOrder() < b->GetDrawOrder();
	});
}

bool Game::RemoveSprite(SpriteComponent* sprite)
{
	auto spritePtr = std::find(mSprites.begin(), mSprites.end(), sprite);

	if (spritePtr == mSprites.end())
	{
		return false;
	}
	else
	{
		mSprites.erase(spritePtr);
		return true;
	}
}

void Game::AddAsteroid(Asteroid* asteroid)
{
	mAsteroids.push_back(asteroid);
}

void Game::RemoveAsteroid(Asteroid* asteroid)
{
	auto asteroidIter = std::find(mAsteroids.begin(), mAsteroids.end(), asteroid);

	mAsteroids.erase(asteroidIter);
}

SDL_Texture* Game::GetTexture(std::string textureName)
{
	auto textureIter = mTextures.unordered_map::find(textureName);

	if (textureIter == mTextures.end())
	{
		std::string file = textureName;
		SDL_Surface* surface = IMG_Load(file.c_str());
		if (surface == nullptr)
		{
			SDL_Log("Texture '%s' is unable to load", file.c_str());
			return nullptr;
		}
		else
		{
			SDL_Texture* texture = SDL_CreateTextureFromSurface(mRenderer, surface);
			SDL_FreeSurface(surface);
			mTextures[textureName] = texture;
			return texture;
		}
	}

	return textureIter->second;
}

void Game::LoadData()
{
	//Stars Background
	Actor* stars = new Actor(this);
	const Vector2 STARS_START = Vector2(512.0f, 384.0f);
	stars->SetPosition(STARS_START);
	SpriteComponent* starsSprite = new SpriteComponent(stars, 0);
	starsSprite->SetTexture(GetTexture("Assets/Stars.png"));

	Ship* ship = new Ship(this);
	const Vector2 SHIP_START = Vector2(300.0f, 300.0f);
	ship->SetPosition(SHIP_START);

	for (int i = 0; i < 10; i++)
	{
		new Asteroid(this);
	}
}

void Game::UnloadData()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	for (std::pair<std::string, SDL_Texture*> textureIter : mTextures)
	{
		SDL_DestroyTexture(textureIter.second);
	}
	mTextures.clear();
}