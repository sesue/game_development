#include "PlayerMove.h"
#include "Game.h"
#include "Actor.h"
#include "Player.h"
#include "Sword.h"
#include "Collider.h"
#include "CollisionComponent.h"
#include "EnemyComponent.h"
#include "AnimatedSprite.h"
#include <SDL2/SDL_image.h>

PlayerMove::PlayerMove(class Actor* owner)
: MoveComponent(owner, 0)
{
	mSword = new Sword(GetGame());
	mSwordCollision = mSword->GetComponent<CollisionComponent>();

	mSprite = mOwner->GetComponent<AnimatedSprite>();
	mPlayerCollision = mOwner->GetComponent<CollisionComponent>();
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
	if (mAttackLifeTime <= 0.0f)
	{
		if (keyState[SDL_SCANCODE_SPACE] && !mSpacePressedPrevFrame)
		{
			mAttackLifeTime = ATTACK_TIME;
			mDirection = Vector2::Zero;
			mOwner->GetGame()->GetAudio()->PlaySound("SwordSlash.wav");
		}
		else if (keyState[SDL_SCANCODE_W])
		{
			mDirection = Vector2(0.0f, -1.0f);
			mFacing = Up;
		}
		else if (keyState[SDL_SCANCODE_D])
		{
			mDirection = Vector2(1.0f, 0.0f);
			mFacing = Right;
		}
		else if (keyState[SDL_SCANCODE_S])
		{
			mDirection = Vector2(0.0f, 1.0f);
			mFacing = Down;
		}
		else if (keyState[SDL_SCANCODE_A])
		{
			mDirection = Vector2(-1.0f, 0.0f);
			mFacing = Left;
		}
		else
		{
			mDirection = Vector2::Zero;
		}
	}
	else
	{
		mDirection = Vector2::Zero;
	}

	mSpacePressedPrevFrame = keyState[SDL_SCANCODE_SPACE];
}

void PlayerMove::UpdateAnimation(float deltaTime)
{
	std::string animation;

	if (mAttackLifeTime > 0.0f)
		animation = "Attack";
	else if (mDirection.x == 0.0f && mDirection.y == 0.0f)
		animation = "Stand";
	else
		animation = "Walk";

	switch (mFacing)
	{
	case Up:
		animation += "Up";
		break;
	case Down:
		animation += "Down";
		break;
	case Right:
		animation += "Right";
		break;
	case Left:
		animation += "Left";
		break;
	}

	if (mAttackLifeTime == ATTACK_TIME)
		mSprite->ResetAnimTimer();

	mSprite->SetAnimation(animation);
}

void PlayerMove::UpdateSword()
{
	Vector2 offset;
	Vector2 collisionSize;

	switch (mFacing)
	{
	case Up:
		offset = Vector2(0, -40);
		collisionSize = Vector2(20, 28);
		break;
	case Down:
		offset = Vector2(0, 40);
		collisionSize = Vector2(20, 28);
		break;
	case Left:
		offset = Vector2(-32, 0);
		collisionSize = Vector2(28, 20);
		break;
	case Right:
		offset = Vector2(32, 0);
		collisionSize = Vector2(28, 20);
		break;
	}

	mSwordCollision->SetSize(collisionSize.x, collisionSize.y);
	mSword->SetPosition(mOwner->GetPosition() + offset);
}

void PlayerMove::Update(float deltaTime)
{
	//Position Update and Velocity
	mOwner->SetPosition(mOwner->GetPosition() + (mDirection * MOVEMENT_SPEED * deltaTime));

	//Animation Update
	UpdateAnimation(deltaTime);

	//Lower Attack Life Timer
	if (mAttackLifeTime > 0.0f)
	{
		mAttackLifeTime -= deltaTime;

		UpdateSword();
	}

	//Collision Check and Logic
	Vector2 offset;
	CollSide side = CollSide::None;
	for (Collider* collider : GetGame()->GetColliders())
	{
		offset = Vector2::Zero;
		side = mPlayerCollision->GetMinOverlap(collider->GetComponent<CollisionComponent>(),
											   offset);

		if (side != CollSide::None)
			mOwner->SetPosition(mOwner->GetPosition() + offset);
	}
	for (EnemyComponent* enemy : GetGame()->GetEnemies())
	{
		CollisionComponent* enemyCollision = enemy->GetCollision();

		if (mAttackLifeTime > 0.0f)
			if (mSwordCollision->Intersect(enemyCollision))
				enemy->TakeDamage();

		offset = Vector2::Zero;
		side = mPlayerCollision->GetMinOverlap(enemyCollision, offset);

		if (side != CollSide::None)
			mOwner->SetPosition(mOwner->GetPosition() + offset);
	}

	//Set Camera Position Last
	mOwner->GetGame()->GetCameraPos() = mOwner->GetPosition() + Vector2(-256.0f, -224.0f);
}
