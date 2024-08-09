//Header Include
#include "PlayerMove.h"

//Components
#include "CollisionComponent.h"
#include "HUD.h"

//Actor Include
#include "Actor.h"
#include "SideBlock.h"
#include "Bullet.h"
#include "Block.h"

//Functional Include
#include "Game.h"
#include "Renderer.h"
#include "Random.h"

PlayerMove::PlayerMove(Actor* owner)
: MoveComponent(owner, 400.0f)
{
	for (int i = 1; i < 8; i++)
	{
		AddSideBlocks(mNextBlockX);
	}

	mShipLoop = mOwner->GetGame()->GetAudio()->PlaySound("ShipLoop.ogg", true);
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
	mVelocity = Vector3::Zero;
	mVelocity.x = X_SPEED;

	if (keyState[SDL_SCANCODE_W])
		mVelocity.z += Z_SPEED;
	if (keyState[SDL_SCANCODE_S])
		mVelocity.z -= Z_SPEED;
	if (keyState[SDL_SCANCODE_D])
		mVelocity.y += Y_SPEED;
	if (keyState[SDL_SCANCODE_A])
		mVelocity.y -= Y_SPEED;
	if (keyState[SDL_SCANCODE_SPACE] && !mPreviousFrameKeyboard[SDL_SCANCODE_SPACE])
	{
		new Bullet(GetGame(), mOwner->GetPosition());
		mOwner->GetGame()->GetAudio()->PlaySound("Shoot.wav");
	}
	if (keyState[SDL_SCANCODE_Q] && !mPreviousFrameKeyboard[SDL_SCANCODE_Q])
	{
		if (mBarrelRollTimer <= 0.0f)
		{
			mBarrelRollTimer = BARREL_ROLL_TIME;
			AdjustShield(false);
		}
	}

	mPreviousFrameKeyboard[SDL_SCANCODE_SPACE] = keyState[SDL_SCANCODE_SPACE];
	mPreviousFrameKeyboard[SDL_SCANCODE_Q] = keyState[SDL_SCANCODE_Q];
}

void PlayerMove::AddSideBlocks(int x)
{
	Game* game = mOwner->GetGame();

	SideBlock* sb = new SideBlock(game, SIDE_INDECIES[(x % 2000) / 500]);
	sb->SetPosition(RIGHT_POSITION + Vector3::UnitX * static_cast<float>(x));
	sb->SetRotation(Math::Pi);

	sb = new SideBlock(game, SIDE_INDECIES[(x % 2000) / 500]);
	sb->SetPosition(LEFT_POSITION + Vector3::UnitX * static_cast<float>(x));

	sb = new SideBlock(game, TOP_INDECIES[(x % 1000) / 500]);
	sb->SetPosition(TOP_POSITION + Vector3::UnitX * static_cast<float>(x));

	sb = new SideBlock(game, BOTTOM_INDEX);
	sb->SetPosition(BOTTOM_POSITION + Vector3::UnitX * static_cast<float>(x));

	mNextBlockX += SIDEBLOCK_REPEAT_DISTANCE;
}

void PlayerMove::AdjustShield(bool hurt = true)
{
	if (hurt)
	{
		mShield -= DAMAGE_PER_HIT;
		mInvicibility = INVICIBILITY_TIME;
		if (mShield > 0)
			mOwner->GetGame()->GetAudio()->PlaySound("ShipHit.wav");
	}
	else
	{
		mOwner->GetGame()->GetAudio()->PlaySound("Boost.wav");
		if (mShield < 3)
			mShield += 1;
	}
}

void PlayerMove::CheckForCollisions(float deltaTime)
{
	//Check for Block Collisions
	CollisionComponent* playerCollision = mOwner->GetComponent<CollisionComponent>();
	for (Block* block : mOwner->GetGame()->GetBlocks())
	{
		if (playerCollision->Intersect(block->GetComponent<CollisionComponent>()))
		{
			if (mInvicibility <= 0.0f)
				AdjustShield(true);

			block->SetState(ActorState::Destroy);
		}
	}

	if (mInvicibility > 0.0f)
		mInvicibility -= deltaTime;

	if (mShield == 1)
		if (mDamageAlert == SoundHandle::Invalid)
			mDamageAlert = mOwner->GetGame()->GetAudio()->PlaySound("DamageAlert.ogg", true);
		else
			mOwner->GetGame()->GetAudio()->ResumeSound(mDamageAlert);
	else if (mOwner->GetGame()->GetAudio()->GetSoundState(mDamageAlert) == SoundState::Playing)
		mOwner->GetGame()->GetAudio()->PauseSound(mDamageAlert);

	if (mShield == 0)
	{
		mOwner->GetGame()->GetAudio()->StopSound(mShipLoop);
		mOwner->GetGame()->GetAudio()->PlaySound("ShipDie.wav");
		mOwner->SetState(ActorState::Paused);
	}
}

void PlayerMove::Update(float deltaTime)
{
	//Barrel Rol Voice Line
	if (mDoABarrelRollVoiceLine <= 0.0f && mShield < MAX_SHIELD)
	{
		mOwner->GetComponent<HUD>()->DoABarrelRoll();
		mDoABarrelRollVoiceLine = Random::GetFloatRange(BARREL_ROLL_VOICE_MIN_TIME,
														BARREL_ROLL_VOICE_MAX_TIME);
	}
	else
	{
		mDoABarrelRollVoiceLine -= deltaTime;
	}

	//Translating Movement (pixels/second)
	if (mSpeedTimer <= 0.0f)
	{
		mSpeedMultiplier += SPEED_MULTIPLIER_INCREMENT;
		mSpeedTimer = SPEED_MULTIPLIER_TIME;
	}
	else
		mSpeedTimer -= deltaTime;

	Vector3 position = mOwner->GetPosition();
	position = position + (mVelocity * mSpeedMultiplier * deltaTime);

	position.y = Math::Clamp(position.y, LOWER_Y, UPPER_Y);
	position.z = Math::Clamp(position.z, LOWER_Z, UPPER_Z);

	mOwner->SetPosition(position);

	//Barrel Roll Movement (radians/second)
	if (mBarrelRollTimer > 0.0f)
	{
		mOwner->SetRotationX(mOwner->GetRotationX() + ROLL_ANGULAR_SPEED * deltaTime);
		mBarrelRollTimer -= deltaTime;
	}
	else
		mOwner->SetRotationX(0);

	CheckForCollisions(deltaTime);

	//Build New SideBlocks if needed
	if (position.x > mNextBlockX - SIDEBLOCK_END_DISTANCE)
		AddSideBlocks(mNextBlockX);

	//Camera Caculation
	Vector3 cameraPos = mOwner->GetPosition() - (Vector3::UnitX * HORIZONTAL_DISTANCE) +
						(Vector3::UnitZ * VERTICAL_DISTANCE);
	//Force Eye position to zero
	cameraPos.z = 0.0f;
	Vector3 targetPos = mOwner->GetPosition() + (Vector3::UnitX * TARGET_DISTANCE);

	mOwner->GetGame()->GetRenderer()->SetViewMatrix(
		Matrix4::CreateLookAt(cameraPos, targetPos, Vector3::UnitZ));
}
