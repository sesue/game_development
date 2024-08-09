#include "Laser.h"
#include "Game.h"
#include "Asteroid.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "Component.h"
#include "Random.h"

Laser::Laser(Game* game)
: Actor(game)
{
	mSprite = new SpriteComponent(this, DRAW_LAYER);
	mSprite->SetTexture(game->GetTexture("Assets/Laser.png"));
	mMovement = new MoveComponent(this);
	mMovement->SetForwardSpeed(FORWARD_SPEED);
}

void Laser::OnUpdate(float deltaTime)
{
	if (mLifetime <= 0.0f)
		mState = ActorState::Destroy;
	else
		mLifetime -= deltaTime;

	if (mPosition.x < 0)
		mPosition.x = mGame->WINDOW_WIDTH;
	if (mPosition.x > mGame->WINDOW_WIDTH)
		mPosition.x = 0;
	if (mPosition.y < 0)
		mPosition.y = mGame->WINDOW_HEIGHT;
	if (mPosition.y > mGame->WINDOW_HEIGHT)
		mPosition.y = 0;

	for (Asteroid* asteroidPtr : mGame->GetAsteroids())
	{
		if (Vector2::Distance(mPosition, asteroidPtr->GetPosition()) <= HIT_DISTANCE)
		{
			mState = ActorState::Destroy;
			asteroidPtr->SetState(ActorState::Destroy);
		}
	}
}