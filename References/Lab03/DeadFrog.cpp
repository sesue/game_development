#include "DeadFrog.h"
#include "Game.h"
#include "SpriteComponent.h"

DeadFrog::DeadFrog(Game* game)
: Actor(game)
{
	mSprite = new SpriteComponent(this);
	mSprite->SetTexture(game->GetTexture("Assets/Dead.png"));
}

void DeadFrog::OnUpdate(float deltaTime)
{
	if (mLifeTime > 0.0f)
		mLifeTime -= deltaTime;
	else
		mState = ActorState::Destroy;
}