//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "Actor.h"
#include "Player.h"
#include "Block.h"
#include "Spawner.h"
#include "Goomba.h"
#include "SpriteComponent.h"
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
	mWindow = SDL_CreateWindow("Mario", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	LoadData();

	return true;
}

void Game::Shutdown()
{
	UnloadData();

	Mix_CloseAudio();
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

void Game::AddBlock(Block* block)
{
	mBlocks.push_back(block);
}

bool Game::RemoveBlock(Block* block)
{
	auto blockPtr = std::find(mBlocks.begin(), mBlocks.end(), block);

	if (blockPtr == mBlocks.end())
	{
		return false;
	}
	else
	{
		mBlocks.erase(blockPtr);
		return true;
	}
}

void Game::AddEnemy(Goomba* enemy)
{
	mEnemies.push_back(enemy);
}

bool Game::RemoveEnemy(Goomba* enemy)
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

Mix_Chunk* Game::GetSound(const std::string& filename)
{
	auto soundIter = mSounds.unordered_map::find(filename);

	if (soundIter == mSounds.end())
	{
		Mix_Chunk* sound = Mix_LoadWAV(filename.c_str());
		if (sound == nullptr)
		{
			SDL_Log("sound '%s' is unable to load", filename.c_str());
			return nullptr;
		}
		else
		{
			mSounds[filename] = sound;
			return sound;
		}
	}

	return soundIter->second;
}

//Load and Unload Data
void Game::LoadData()
{
	//Background Code
	Actor* background = new Actor(this);
	background->SetPosition(mBackgroundPosition);
	SpriteComponent* backgroundSprite = new SpriteComponent(background, 0);
	backgroundSprite->SetTexture(GetTexture("Assets/Background.png"));

	//Load Level File
	std::ifstream level;

	level.open("Assets/Level1.txt");

	for (int y = 0; !level.eof(); y++)
	{
		char line[255] = "";
		level.getline(line, 255);
		for (int x = 0; line[x] != '\0'; x++)
		{
			if (line[x] >= 'A' && line[x] <= 'I')
			{
				const Vector2 POSITION(LEVEL_TOP_LEFT.x + GRID_STEP.x * x,
									   LEVEL_TOP_LEFT.y + GRID_STEP.y * y);
				Block* block = new Block(this, line[x]);
				block->SetPosition(POSITION);
			}
			else if (line[x] == 'P')
			{
				const Vector2 POSITION(LEVEL_TOP_LEFT.x + GRID_STEP.x * x,
									   LEVEL_TOP_LEFT.y + GRID_STEP.y * y);
				mPlayer = new Player(this);
				mPlayer->SetPosition(POSITION);
			}
			else if (line[x] == 'Y')
			{
				const Vector2 POSITION(LEVEL_TOP_LEFT.x + GRID_STEP.x * x,
									   LEVEL_TOP_LEFT.y + GRID_STEP.y * y);
				Spawner* spawner = new Spawner(this);
				spawner->SetPosition(POSITION);
			}
		}
	}

	mMusicChannel = Mix_PlayChannel(-1, GetSound("Assets/Sounds/Music.ogg"), -1);
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

	for (std::pair<std::string, Mix_Chunk*> soundIter : mSounds)
		Mix_FreeChunk(soundIter.second);
	mSounds.clear();
}