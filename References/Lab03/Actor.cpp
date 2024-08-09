#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
: mGame(game)
, mState(ActorState::Active)
, mPosition(Vector2::Zero)
, mScale(1.0f)
, mRotation(0.0f)
{
	mGame->AddActor(this);
}

Actor::~Actor()
{
	mGame->RemoveActor(this);
	for (Component* componentPtr : mComponents)
	{
		delete componentPtr;
	}
	mComponents.clear();
}

void Actor::Update(float deltaTime)
{
	// TODO
	if (mState == ActorState::Active)
	{
		for (class Component* compPtr : mComponents)
		{
			compPtr->Update(deltaTime);
		}
	}

	if (mPosition.x < 0)
		mPosition.x = mGame->WINDOW_WIDTH;
	if (mPosition.x > mGame->WINDOW_WIDTH)
		mPosition.x = 0;
	if (mPosition.y < 0)
		mPosition.y = mGame->WINDOW_HEIGHT;
	if (mPosition.y > mGame->WINDOW_HEIGHT)
		mPosition.y = 0;

	OnUpdate(deltaTime);
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState)
{
	// TODO
	if (mState == ActorState::Active)
	{
		for (class Component* compPtr : mComponents)
		{
			compPtr->ProcessInput(keyState);
		}
		OnProcessInput(keyState);
	}
}

void Actor::OnProcessInput(const Uint8* keyState)
{
}

void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}
