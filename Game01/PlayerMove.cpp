#include "PlayerMove.h"
#include "Game.h"
#include "Goomba.h"
#include "Actor.h"
#include "Player.h"
#include "Block.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
#include <SDL2/SDL_image.h>

PlayerMove::PlayerMove(class Actor* owner)
: MoveComponent(owner, 0)
{
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
	mXSpeed = 0;

	if (keyState[SDL_SCANCODE_A])
		mXSpeed -= INITIAL_HORITZONTAL_SPEED;
	if (keyState[SDL_SCANCODE_D])
		mXSpeed += INITIAL_HORITZONTAL_SPEED;
	if (keyState[SDL_SCANCODE_SPACE] && !mSpacePressed && !mInAir)
	{
		mYSpeed = INITIAL_JUMP_SPEED;
		mSpacePressed = true;
		mInAir = true;
		Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/Jump.wav"), 0);
	}
	else if (!keyState[SDL_SCANCODE_SPACE] && !mInAir)
		mSpacePressed = false;
}

void PlayerMove::KillPlayer()
{
	static_cast<Player*>(mOwner)->GetSprite()->SetAnimation("dead");
	mOwner->SetState(ActorState::Paused);

	Mix_HaltChannel(GetGame()->GetMusicChannel());
	Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/Dead.wav"), 0);
}

void PlayerMove::PlayerWin()
{
	mOwner->SetState(ActorState::Paused);

	Mix_HaltChannel(GetGame()->GetMusicChannel());
	Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/StageClear.wav"), 0);
}

void PlayerMove::UpdateAnimation(float deltaTime)
{
	if (!mInAir)
	{
		if (mXSpeed > 0.0f)
			static_cast<Player*>(mOwner)->GetSprite()->SetAnimation("runRight");
		else if (mXSpeed < 0.0f)
			static_cast<Player*>(mOwner)->GetSprite()->SetAnimation("runLeft");
		else
			static_cast<Player*>(mOwner)->GetSprite()->SetAnimation("idle");
	}
	else
	{
		if (mXSpeed > 0.0f)
			static_cast<Player*>(mOwner)->GetSprite()->SetAnimation("jumpRight");
		else if (mXSpeed < 0.0f)
			static_cast<Player*>(mOwner)->GetSprite()->SetAnimation("jumpLeft");
		else
		{
			if (static_cast<Player*>(mOwner)->GetSprite()->GetAnimName() == "runRight" ||
				static_cast<Player*>(mOwner)->GetSprite()->GetAnimName() == "jumpRight" ||
				static_cast<Player*>(mOwner)->GetSprite()->GetAnimName() == "idle")
				static_cast<Player*>(mOwner)->GetSprite()->SetAnimation("jumpRight");
			else
				static_cast<Player*>(mOwner)->GetSprite()->SetAnimation("jumpLeft");
		}
	}
}

void PlayerMove::Update(float deltaTime)
{
	CollisionComponent* playerCollision = static_cast<Player*>(mOwner)->GetCollision();

	//Set Position Based on X-Y-Velocity
	mOwner->SetPosition(Vector2(mOwner->GetPosition().x + (mXSpeed * deltaTime),
								mOwner->GetPosition().y + (mYSpeed * deltaTime)));

	//Check for Collisions
	mInAir = true;
	for (Block* block : GetGame()->GetBlocks())
	{
		Vector2 offset = Vector2::Zero;
		CollSide side = playerCollision->GetMinOverlap(block->GetCollision(), offset);
		if (side != CollSide::None)
		{
			if (side == CollSide::Top && mYSpeed > 0.0f)
			{
				mYSpeed = 0;
				mInAir = false;
			}
			if (side == CollSide::Bottom && mYSpeed < 0.0f)
			{
				Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/Bump.wav"), 0);
				mYSpeed = 0;
			}

			mOwner->SetPosition(mOwner->GetPosition() + offset);
		}
	}

	//Check Goombas
	for (Goomba* goomba : GetGame()->GetEnemies())
	{
		Vector2 offset = Vector2::Zero;
		CollSide side = playerCollision->GetMinOverlap(goomba->GetCollision(), offset);
		if (side != CollSide::None)
		{
			if (side == CollSide::Top ||
				((side == CollSide::Right || side == CollSide::Left) && mInAir))
			{
				goomba->Stomp();
				Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/Stomp.wav"), 0);
				mYSpeed = INITIAL_JUMP_SPEED / 2.0f;
				mInAir = true;
			}
			else if (!(goomba->GetStomped()))
				KillPlayer();
		}
	}

	//Apply Gravity
	mYSpeed += GetGame()->GAME_GRAVITY * deltaTime;

	//Death by Falling
	if (mOwner->GetPosition().y > GetGame()->GAME_FLOOR)
		KillPlayer();

	//Win Condition
	if (mOwner->GetPosition().x > GetGame()->FLAG_POLE_X)
		PlayerWin();

	//Set Proper Animation
	if (mOwner->GetState() == ActorState::Active)
		UpdateAnimation(deltaTime);

	//Camera Positioning
	float currCameraX = GetGame()->GetCameraPos().x;
	float newCameraX = mOwner->GetPosition().x - static_cast<float>(GetGame()->WINDOW_WIDTH / 2);

	if (newCameraX > currCameraX)
		GetGame()->GetCameraPos().x = newCameraX;
	if (currCameraX < 0.0f)
		GetGame()->GetCameraPos().x = 0.0f;

	if (mOwner->GetPosition().x < currCameraX)
	{
		mOwner->SetPosition(Vector2(currCameraX, mOwner->GetPosition().y));
	}
}
