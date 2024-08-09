//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

//Header Include
#include "Game.h"

//Library Includes
#include <algorithm>
#include <fstream>

//Actor Includes
#include "Actor.h"
#include "Player.h"
#include "SideBlock.h"
#include "Block.h"

//Utility Includes
#include "Renderer.h"
#include "Random.h"

Game::Game()
{
}

bool Game::Initialize()
{
	Random::Init();

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT))
	{
		SDL_Log("Unable to initialize Renderer.");
		return false;
	}

	mAudio = new AudioSystem();

	LoadData();

	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
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
			mIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	std::vector<Actor*> copy = mActors;
	for (auto actor : copy)
	{
		actor->ProcessInput(state);
	}

	mAudio->ProcessInput(state);
}

void Game::UpdateGame()
{
	// Compute delta time
	Uint32 tickNow = SDL_GetTicks();
	// Wait until 16ms has elapsed since last frame
	while (tickNow - mTicksCount < 16)
	{
		tickNow = SDL_GetTicks();
	}

	// For 3D the games, force delta time to 16ms even if slower
	float deltaTime = 0.016f;
	mTicksCount = tickNow;

	mAudio->Update(deltaTime);

	//Load Blocks in front of Player
	//Build New SideBlocks if needed
	if (mPlayer->GetPosition().x > mNextBlockLoadX - BLOCK_LOAD_DISTANCE)
	{
		LoadBlocks("Assets/Blocks/" + std::to_string(mNextBlockFile) + ".txt");
		if (mNextBlockFile == 20)
			mIsRandomBlockFile = true;

		if (!mIsRandomBlockFile)
			mNextBlockFile++;
		else
			mNextBlockFile = Random::GetIntRange(MIN_BLOCK_FILE, MAX_BLOCK_FILE);
	}

	// Make copy of actor vector
	// (iterate over this in case new actors are created)
	std::vector<Actor*> copy = mActors;
	// Update all actors
	for (auto actor : copy)
	{
		actor->Update(deltaTime);
	}

	// Add any actors to destroy to a temp vector
	std::vector<Actor*> destroyActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == ActorState::Destroy)
		{
			destroyActors.emplace_back(actor);
		}
	}

	// Delete the destroyed actors (which will
	// remove them from mActors)
	for (auto actor : destroyActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	mRenderer->Draw();
}

void Game::LoadData()
{
	// Precache all the sounds (do not remove)
	mAudio->CacheAllSounds();

	//Create Player
	mPlayer = new Player(this);
	Matrix4 perspectiveMatrix = Matrix4::CreatePerspectiveFOV(1.22f, WINDOW_WIDTH, WINDOW_HEIGHT,
															  10.0f, 10000.0f);
	mRenderer->SetProjectionMatrix(perspectiveMatrix);
	Matrix4 viewMatrix = Matrix4::CreateLookAt(Vector3(-300, 0, 0), Vector3(20, 0, 0),
											   Vector3::UnitZ);
	mRenderer->SetViewMatrix(viewMatrix);

	mAudio->PlaySound("Music.ogg", true);
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}
}

void Game::Shutdown()
{
	UnloadData();
	delete mAudio;
	mRenderer->Shutdown();
	delete mRenderer;
	SDL_Quit();
}

void Game::LoadBlocks(std::string file)
{
	//Load Level File
	std::ifstream level;

	level.open(file);

	for (int z = 0; !level.eof(); z++)
	{
		char line[255] = "";
		level.getline(line, 255);
		for (int y = 0; line[y] != '\0'; y++)
		{
			if (line[y] != '.')
			{
				Block* block = nullptr;
				if (line[y] == 'A')
					block = new Block(this, false);
				else if (line[y] == 'B')
					block = new Block(this, true);
				const Vector3 POSITION(mNextBlockLoadX, BLOCK_Y_START + (y * BLOCK_Y_INCREMENT),
									   BLOCK_Z_START + (z * BLOCK_Z_INCREMENT));
				block->SetPosition(POSITION);
			}
		}
	}

	mNextBlockLoadX += BLOCK_X_INCREMENT;
}

void Game::AddActor(Actor* actor)
{
	mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		mActors.erase(iter);
	}
}

void Game::AddBlock(Block* block)
{
	mBlocks.emplace_back(block);
}

void Game::RemoveBlock(Block* block)
{
	auto iter = std::find(mBlocks.begin(), mBlocks.end(), block);
	if (iter != mBlocks.end())
	{
		mBlocks.erase(iter);
	}
}
