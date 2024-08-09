#include "Ship.h"
#include "Game.h"
#include "Laser.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "Component.h"
#include <algorithm>

Ship::Ship(Game* game)
: Actor(game)
{
	mSprite = new SpriteComponent(this, DRAW_LAYER);
	mSprite->SetTexture(game->GetTexture("Assets/Ship.png"));
	mMovement = new MoveComponent(this);
}

void Ship::OnProcessInput(const Uint8* keyState)
{
	float forwardSpeed = 0.0f;
	float angularSpeed = 0.0f;

	//Set Speed based on Input
	if (keyState[SDL_SCANCODE_W])
		forwardSpeed += MAX_FORWARD_SPEED;
	if (keyState[SDL_SCANCODE_S])
		forwardSpeed -= MAX_FORWARD_SPEED;
	if (keyState[SDL_SCANCODE_A])
		angularSpeed += MAX_ANGULAR_SPEED;
	if (keyState[SDL_SCANCODE_D])
		angularSpeed -= MAX_ANGULAR_SPEED;

	mMovement->SetForwardSpeed(forwardSpeed);
	mMovement->SetAngularSpeed(angularSpeed);

	//Set Text based on Speed
	if (forwardSpeed != 0.0f)
		mSprite->SetTexture(mGame->GetTexture("Assets/ShipThrust.png"));
	else
		mSprite->SetTexture(mGame->GetTexture("Assets/Ship.png"));

	//Laser Firing
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
	{
		Laser* laser = new Laser(mGame);
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());
		mLaserCooldown = 1.0f;
	}
}

void Ship::OnUpdate(float deltaTime)
{
	if (mPosition.x < 0)
		mPosition.x = mGame->WINDOW_WIDTH;
	if (mPosition.x > mGame->WINDOW_WIDTH)
		mPosition.x = 0;
	if (mPosition.y < 0)
		mPosition.y = mGame->WINDOW_HEIGHT;
	if (mPosition.y > mGame->WINDOW_HEIGHT)
		mPosition.y = 0;

	mLaserCooldown -= deltaTime;
}