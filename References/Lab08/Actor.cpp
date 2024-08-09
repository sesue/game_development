#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
: mGame(game)
, mState(ActorState::Active)
, mPosition(Vector3::Zero)
, mScale(1.0f)
, mRotation(0.0f)
, mRotationX(0.0f)
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
	if (mState == ActorState::Active)
	{
		for (class Component* compPtr : mComponents)
		{
			compPtr->Update(deltaTime);
		}
	}

	OnUpdate(deltaTime);
	CalcWorldTransform();
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState)
{
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

void Actor::CalcWorldTransform()
{
	Matrix4 scaleMatrix = Matrix4::CreateScale(mScale);
	Matrix4 rotationMatrix = Matrix4::CreateRotationZ(mRotation);
	Matrix4 rotationXMatrix = Matrix4::CreateRotationX(mRotationX);
	Matrix4 translationMatrix = Matrix4::CreateTranslation(mPosition);

	mWorldTransform = scaleMatrix * rotationMatrix * rotationXMatrix * translationMatrix;
}