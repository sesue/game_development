#include "Asteroid.h"
#include "Game.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "Component.h"
#include "Random.h"

Asteroid::Asteroid(Game* game)
: Actor(game)
{
	mSprite = new SpriteComponent(this, DRAW_LAYER);
	mSprite->SetTexture(game->GetTexture("Assets/Asteroid.png"));
	mMovement = new MoveComponent(this);
	mMovement->SetForwardSpeed(FORWARD_SPEED);

	mRotation = Random::GetFloatRange(0.0f, Math::TwoPi);
	mPosition = Random::GetVector(mGame->WINDOW_MIN_VECTOR, mGame->WINDOW_MAX_VECTOR);

	game->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
	mGame->RemoveAsteroid(this);
}

void Asteroid::OnUpdate(float deltaTime)
{
	if (mPosition.x < 0)
		mPosition.x = mGame->WINDOW_WIDTH;
	if (mPosition.x > mGame->WINDOW_WIDTH)
		mPosition.x = 0;
	if (mPosition.y < 0)
		mPosition.y = mGame->WINDOW_HEIGHT;
	if (mPosition.y > mGame->WINDOW_HEIGHT)
		mPosition.y = 0;
}