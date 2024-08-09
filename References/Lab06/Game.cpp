//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "AudioSystem.h"
#include "Actor.h"
#include "Player.h"
#include "Collider.h"
#include "Bush.h"
#include "Soldier.h"
#include "PathFinder.h"
#include "PathNode.h"
#include "SpriteComponent.h"
#include "TileBGComponent.h"
#include "CSVHelper.h"
#include "CollisionComponent.h"
#include "Random.h"
#include <fstream>
#include <string>
#include <SDL2/SDL_image.h>

//Public Functions for Game
bool Game::Initialize()
{
	Random::Init();

	//Setup SDL, Window, and Renderer for Game
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO))
	{
		return false;
	}
	mWindow = SDL_CreateWindow("Zelda", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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
	mAudioSystem = new AudioSystem();

	LoadData();

	return true;
}

void Game::Shutdown()
{
	UnloadData();

	delete mAudioSystem;
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

//Game Loop Functions
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
	if (SoundState::Stopped == mAudioSystem->GetSoundState(mSoundHandle))
		mSoundHandle = mAudioSystem->PlaySound("MusicLoop.ogg", true);

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

	mAudioSystem->Update(deltaTime);

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

//Add and Remove Functions
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

void Game::AddCollider(Collider* collider)
{
	mColliders.push_back(collider);
}

bool Game::RemoveCollider(Collider* collider)
{
	auto colliderPtr = std::find(mColliders.begin(), mColliders.end(), collider);

	if (colliderPtr == mColliders.end())
	{
		return false;
	}
	else
	{
		mColliders.erase(colliderPtr);
		return true;
	}
}

void Game::AddEnemyComponent(EnemyComponent* enemy)
{
	mEnemies.push_back(enemy);
}

bool Game::RemoveEnemyComponent(EnemyComponent* enemy)
{
	auto enemyPtr = std::find(mEnemies.begin(), mEnemies.end(), enemy);

	if (enemyPtr == mEnemies.end())
	{
		return false;
	}
	else
	{
		mEnemies.erase(enemyPtr);
		return true;
	}
}

//Get Functions
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

void Game::LoadObjects()
{
	//Construct PathFinder
	mPathFinder = new PathFinder(this);

	//Load Level File
	std::ifstream file(OBJECT_FILE_PATH);

	std::string line;
	while (getline(file, line))
	{
		std::vector<std::string> objectData = CSVHelper::Split(line);
		Actor* actor = nullptr;

		if (objectData[0] == "Player")
			actor = new Player(this);
		else if (objectData[0] == "Collider")
			actor = new Collider(this, stof(objectData[3]), stof(objectData[4]));
		else if (objectData[0] == "Bush")
			actor = new Bush(this);
		else if (objectData[0] == "Soldier")
			actor = new Soldier(this,
								mPathFinder->GetPathNode(stoi(objectData[5]), stoi(objectData[6])),
								mPathFinder->GetPathNode(stoi(objectData[7]), stoi(objectData[8])));

		//objectData Index: 1 = x, 2 = y, 3 = width, 4 = height
		if (actor != nullptr)
			actor->SetPosition(Vector2(stof(objectData[1]) + (stof(objectData[3]) / 2),
									   stof(objectData[2]) + (stof(objectData[4]) / 2)));
	}
}

//Load and Unload Data
void Game::LoadData()
{
	mAudioSystem->CacheAllSounds();
	mSoundHandle = mAudioSystem->PlaySound("MusicStart.ogg");

	Actor* background = new Actor(this);
	TileBGComponent* tileComponent = new TileBGComponent(background);
	tileComponent->SetTexture(GetTexture(TEXTURE_FILE_PATH));
	tileComponent->LoadTileCSV(BACKGROUND_FILE_PATH, TILE_WIDTH, TILE_HEIGHT);

	LoadObjects();
}

void Game::UnloadData()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	for (std::pair<std::string, SDL_Texture*> textureIter : mTextures)
		SDL_DestroyTexture(textureIter.second);
	mTextures.clear();
}