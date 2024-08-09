#include "Frog.h"
#include "DeadFrog.h"
#include "Game.h"
#include "Vehicle.h"
#include "Log.h"
#include "WrappingMove.h"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include "Component.h"
#include <algorithm>

Frog::Frog(Game* game, const Vector2 startPosition)
: Actor(game)
{
	SetPosition(startPosition);
	mStartPosition = startPosition;

	mSprite = new SpriteComponent(this, DRAW_LAYER);
	mSprite->SetTexture(game->GetTexture("Assets/Frog.png"));

	mCollision = new CollisionComponent(this);
	mCollision->SetSize(FROG_COLLISION_WIDTH, FROG_COLLISION_HEIGHT);
}

void Frog::OnProcessInput(const Uint8* keyState)
{
	float xChange = 0;
	float yChange = 0;

	//Set Speed based on Input
	if (keyState[SDL_SCANCODE_W] && !mPreviousFrameKeyboard[SDL_SCANCODE_W])
		yChange -= STEP_DISTANCE;
	if (keyState[SDL_SCANCODE_S] && !mPreviousFrameKeyboard[SDL_SCANCODE_S])
		yChange += STEP_DISTANCE;
	if (keyState[SDL_SCANCODE_A] && !mPreviousFrameKeyboard[SDL_SCANCODE_A])
		xChange -= STEP_DISTANCE;
	if (keyState[SDL_SCANCODE_D] && !mPreviousFrameKeyboard[SDL_SCANCODE_D])
		xChange += STEP_DISTANCE;

	mPreviousFrameKeyboard[SDL_SCANCODE_W] = keyState[SDL_SCANCODE_W];
	mPreviousFrameKeyboard[SDL_SCANCODE_S] = keyState[SDL_SCANCODE_S];
	mPreviousFrameKeyboard[SDL_SCANCODE_A] = keyState[SDL_SCANCODE_A];
	mPreviousFrameKeyboard[SDL_SCANCODE_D] = keyState[SDL_SCANCODE_D];

	SetPosition(Vector2(Math::Clamp(GetPosition().x + xChange, 32.0f, 416.0f),
						Math::Clamp(GetPosition().y + yChange, 80.0f, 464.0f)));
}

void Frog::OnUpdate(float deltaTime)
{
	//Vehicle Checks
	for (Vehicle* vehicle : mGame->GetVehicles())
	{
		if (mCollision->Intersect(vehicle->GetCollision()))
		{
			DeadFrog* deadFrog = new DeadFrog(mGame);
			deadFrog->SetPosition(GetPosition());
			SetPosition(mStartPosition);
		}
	}

	//Log Checks
	bool ridingLog = false;
	bool inWater = (GetPosition().y > 90 && GetPosition().y < 255);

	for (Log* log : mGame->GetLogs())
	{
		Vector2 offset = Vector2::Zero;
		CollSide side = mCollision->GetMinOverlap(log->GetCollision(), offset);
		if (side != CollSide::None)
		{
			if (side == CollSide::Left)
				SetPosition(GetPosition() + offset + Vector2(32.0f, 0.0f));
			if (side == CollSide::Right)
				SetPosition(GetPosition() + offset - Vector2(32.0f, 0.0f));

			Vector2 velocity =
				Vector2(log->GetMove()->mDirection.x * log->GetMove()->GetForwardSpeed(),
						log->GetMove()->mDirection.y);
			SetPosition(Vector2(GetPosition().x, log->GetPosition().y));
			SetPosition(GetPosition() + (velocity * deltaTime));
			ridingLog = true;
		}
	}

	//Drowning Check
	if (!ridingLog && inWater)
	{
		DeadFrog* deadFrog = new DeadFrog(mGame);
		deadFrog->SetPosition(GetPosition());
		SetPosition(mStartPosition);
	}

	//Check for Goal Collision for Win
	if (mCollision->Intersect(mGame->GetGoalCollision()))
	{
		SetPosition(mGame->GetGoal()->GetPosition());
		SetState(ActorState::Paused);
	}
	else if (GetPosition().y < 90)
	{
		DeadFrog* deadFrog = new DeadFrog(mGame);
		deadFrog->SetPosition(GetPosition());
		SetPosition(mStartPosition);
	}
}