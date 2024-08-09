//Header Include
#include "PlayerMove.h"

//Components
#include "CollisionComponent.h"
#include "CameraComponent.h"
#include "HealthComponent.h"

//Actor Include
#include "Actor.h"
#include "Player.h"
#include "Block.h"

//Functional Include
#include "Game.h"
#include "Renderer.h"
#include "Random.h"
#include "Crosshair.h"
#include "Portal.h"
#include "SegmentCast.h"

PlayerMove::PlayerMove(Actor* owner)
: MoveComponent(owner, 0.0f)
{
	mCameraComponent = owner->GetComponent<CameraComponent>();

	mCrosshair = new Crosshair(owner);

	AudioSystem* audio = owner->GetGame()->GetAudio();
	mFootsteps = audio->PlaySound(FOOTSTEPS_SOUND, true);
	audio->PauseSound(mFootsteps);

	ChangeState(OnGround);
}

PlayerMove::~PlayerMove()
{
	mOwner->GetGame()->GetAudio()->StopSound(mFootsteps);
}

void PlayerMove::ProcessInput(const Uint8* keyState, Uint32 mouseButtons,
							  const Vector2& relativeMouse)
{
	if (mOwner->GetComponent<HealthComponent>()->IsDead())
	{
		mCameraComponent->SetPitchSpeed(0.0f);
		return;
	}

	Vector3 ownerForward = mOwner->GetForward();
	Vector3 ownerRight = mOwner->GetRight();

	if (keyState[SDL_SCANCODE_W])
		AddForce(ownerForward * MOVEMENT_SCALE);
	if (keyState[SDL_SCANCODE_S])
		AddForce(ownerForward * -MOVEMENT_SCALE);
	if (keyState[SDL_SCANCODE_A])
		AddForce(ownerRight * -MOVEMENT_SCALE);
	if (keyState[SDL_SCANCODE_D])
		AddForce(ownerRight * MOVEMENT_SCALE);

	if (keyState[SDL_SCANCODE_SPACE] && !mPreviousFrameKeyboard["SDL_SCANCODE_SPACE"] &&
		mCurrentState == OnGround)
	{
		AddForce(mJumpForce);
		ChangeState(Jump);
		mOwner->GetGame()->GetAudio()->PlaySound(JUMP_SOUND);
	}

	if (static_cast<Player*>(mOwner)->HasGun())
	{
		if (mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT) &&
			!mPreviousFrameKeyboard["SDL_BUTTON_LEFT"])
			CreatePortal(true);
		if (mouseButtons & SDL_BUTTON(SDL_BUTTON_RIGHT) &&
			!mPreviousFrameKeyboard["SDL_BUTTON_RIGHT"])
			CreatePortal(false);
		if (keyState[SDL_SCANCODE_R] && !mPreviousFrameKeyboard["SDL_SCANCODE_R"])
		{
			Game* game = mOwner->GetGame();
			if (game->GetBluePortal() != nullptr || game->GetOrangePortal() != nullptr)
				mOwner->GetGame()->GetAudio()->PlaySound(PORTAL_CLOSE_SOUND);
			if (game->GetBluePortal() != nullptr)
			{
				game->GetBluePortal()->SetState(ActorState::Destroy);
				game->SetBluePortal(nullptr);
				mBlueExists = false;
			}
			if (game->GetOrangePortal() != nullptr)
			{
				game->GetOrangePortal()->SetState(ActorState::Destroy);
				game->SetOrangePortal(nullptr);
				mOrangeExists = false;
			}

			mCrosshair->SetState(CrosshairState::Default);
		}
	}

	mPreviousFrameKeyboard["SDL_SCANCODE_SPACE"] = keyState[SDL_SCANCODE_SPACE];
	mPreviousFrameKeyboard["SDL_SCANCODE_R"] = keyState[SDL_SCANCODE_R];
	mPreviousFrameKeyboard["SDL_BUTTON_LEFT"] = mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT);
	mPreviousFrameKeyboard["SDL_BUTTON_RIGHT"] = mouseButtons & SDL_BUTTON(SDL_BUTTON_RIGHT);

	float angularSpeed = relativeMouse.x / 500.0f * Math::Pi * 10.0f;
	SetAngularSpeed(angularSpeed);

	float pitchSpeed = relativeMouse.y / 500.0f * Math::Pi * 10.0f;
	mCameraComponent->SetPitchSpeed(pitchSpeed);
}

void PlayerMove::Update(float deltaTime)
{
	if (mOwner->GetComponent<HealthComponent>()->IsDead())
	{
		if (mOwner->GetGame()->GetAudio()->GetSoundState(mDeathSound) == SoundState::Playing)
			return;
		else
			mOwner->GetGame()->ReloadLevel();
	}

	if (mOwner->GetPosition().z <= RELOAD_FLOOR)
		mOwner->GetComponent<HealthComponent>()->TakeDamage(Math::Infinity, mOwner->GetPosition());

	switch (mCurrentState)
	{
	case OnGround:
		UpdateOnGround(deltaTime);
		break;
	case Jump:
		UpdateJump(deltaTime);
		break;
	case Falling:
		UpdateFalling(deltaTime);
		break;
	}

	AudioSystem* audio = mOwner->GetGame()->GetAudio();
	if (mCurrentState == MoveState::OnGround && mVelocity.Length() > 50.0f &&
		audio->GetSoundState(mFootsteps) == SoundState::Paused)
		audio->ResumeSound(mFootsteps);
	else
		audio->PauseSound(mFootsteps);
}

void PlayerMove::UpdateOnGround(float deltaTime)
{
	PhysicsUpdate(deltaTime);
	if (UpdatePortalTeleport(deltaTime))
		return;

	CollisionComponent* selfCollider = mOwner->GetComponent<CollisionComponent>();
	bool topCollision = false;
	for (Actor* actor : mOwner->GetGame()->GetColliders())
	{
		CollisionComponent* collider = actor->GetComponent<CollisionComponent>();
		if (FixCollision(selfCollider, collider) == CollSide::Top)
			topCollision = true;
	}

	if (!topCollision)
		ChangeState(Falling);
}

void PlayerMove::UpdateJump(float deltaTime)
{
	AddForce(mGravity);

	PhysicsUpdate(deltaTime);
	if (UpdatePortalTeleport(deltaTime))
		return;

	CollisionComponent* selfCollider = mOwner->GetComponent<CollisionComponent>();
	for (Actor* actor : mOwner->GetGame()->GetColliders())
	{
		CollisionComponent* collider = actor->GetComponent<CollisionComponent>();
		if (FixCollision(selfCollider, collider) == CollSide::Bottom)
			mVelocity.z = 0.0f;
	}

	if (mVelocity.z <= 0.0f)
		ChangeState(Falling);
}

void PlayerMove::UpdateFalling(float deltaTime)
{
	AddForce(mGravity);

	PhysicsUpdate(deltaTime);
	if (UpdatePortalTeleport(deltaTime))
		return;

	CollisionComponent* selfCollider = mOwner->GetComponent<CollisionComponent>();
	bool topCollision = false;
	for (Actor* actor : mOwner->GetGame()->GetColliders())
	{
		CollisionComponent* collider = actor->GetComponent<CollisionComponent>();
		if (FixCollision(selfCollider, collider) == CollSide::Top)
			topCollision = true;
	}

	if (topCollision && mVelocity.z <= 0.0f)
	{
		mVelocity.z = 0.0f;
		ChangeState(OnGround);
		teleportFall = false;
		mOwner->GetGame()->GetAudio()->PlaySound(LAND_SOUND);
	}
}

void PlayerMove::PhysicsUpdate(float deltaTime)
{
	mAcceleration = mPendingForces * (1.0f / mMass);
	mVelocity += mAcceleration * deltaTime;

	if (!teleportFall)
		FixXYVelocity();
	
	if (mVelocity.z < -1000.0f)
		mVelocity.z = -1000.0f;

	mOwner->SetPosition(mOwner->GetPosition() + mVelocity * deltaTime);

	mOwner->SetRotation(mOwner->GetRotation() + (mAngularSpeed * deltaTime));

	mPendingForces = Vector3::Zero;
}

void PlayerMove::FixXYVelocity()
{
	Vector2 xyVelocity = Vector2(mVelocity.x, mVelocity.y);

	if (xyVelocity.Length() > MAX_SPEED)
	{
		xyVelocity = Vector2::Normalize(xyVelocity) * MAX_SPEED;
	}

	if (mCurrentState == OnGround)
	{
		if (Math::NearlyZero(mAcceleration.x) || ((mAcceleration.x * xyVelocity.x) < 0))
			xyVelocity.x = xyVelocity.x * BRAKING_FACTOR;
		if (Math::NearlyZero(mAcceleration.y) || ((mAcceleration.y * xyVelocity.y) < 0))
			xyVelocity.y = xyVelocity.y * BRAKING_FACTOR;
	}

	mVelocity.x = xyVelocity.x;
	mVelocity.y = xyVelocity.y;
}

CollSide PlayerMove::FixCollision(CollisionComponent* self, CollisionComponent* collider)
{
	Vector3 offset = Vector3::Zero;
	CollSide side = self->GetMinOverlap(collider, offset);

	if (side != CollSide::None)
		mOwner->SetPosition(mOwner->GetPosition() + offset);

	return side;
}

void PlayerMove::CreatePortal(bool isBlue)
{
	Game* game = mOwner->GetGame();

	Vector3 nearPoint = game->GetRenderer()->Unproject(Vector3(0.0f, 0.0f, 0.0f));
	Vector3 farPoint = game->GetRenderer()->Unproject(Vector3(0.0f, 0.0f, 1.0f));
	Vector3 directionVector = Vector3::Normalize(farPoint - nearPoint);

	const LineSegment lineSeg = LineSegment(nearPoint, nearPoint + (directionVector * 1000.0f));
	CastInfo outInfo;

	if (SegmentCast(game->GetColliders(), lineSeg, outInfo))
	{
		Block* b = dynamic_cast<Block*>(outInfo.mActor);
		if (b != nullptr)
		{
			Portal* portal = new Portal(game, isBlue, outInfo.mNormal);
			portal->SetPosition(outInfo.mPoint);
			portal->SetQuat(game->CreateArbitraryQuaternion(Vector3::UnitX, outInfo.mNormal));

			Portal* oldPortal = nullptr;

			if (isBlue)
			{
				oldPortal = game->GetBluePortal();
				game->SetBluePortal(portal);
				mBlueExists = true;
				mOwner->GetGame()->GetAudio()->PlaySound(SHOOT_BLUE_SOUND);
			}
			else
			{
				oldPortal = game->GetOrangePortal();
				game->SetOrangePortal(portal);
				mOrangeExists = true;
				mOwner->GetGame()->GetAudio()->PlaySound(SHOOT_ORANGE_SOUND);
			}

			if (oldPortal != nullptr)
			{
				oldPortal->SetState(ActorState::Destroy);
			}

			if (mBlueExists && !mOrangeExists)
				mCrosshair->SetState(CrosshairState::BlueFill);
			else if (!mBlueExists && mOrangeExists)
				mCrosshair->SetState(CrosshairState::OrangeFill);
			if (mBlueExists && mOrangeExists)
				mCrosshair->SetState(CrosshairState::BothFill);

			portal->Update(0.0f);
		}
		else 
			mOwner->GetGame()->GetAudio()->PlaySound(PORTAL_FAIL_SOUND);
	}
	else
		mOwner->GetGame()->GetAudio()->PlaySound(PORTAL_FAIL_SOUND);
}

bool PlayerMove::UpdatePortalTeleport(float deltaTime)
{
	Game* game = mOwner->GetGame();
	Portal* bluePortal = game->GetBluePortal();
	Portal* orangePortal = game->GetOrangePortal();

	if (bluePortal == nullptr || orangePortal == nullptr)
		return false;

	if (mTeleportCooldown > 0.0f)
	{
		mTeleportCooldown -= deltaTime;
		return false;
	}

	CollisionComponent* playerCollision = mOwner->GetComponent<CollisionComponent>();
	Portal* entry = nullptr;
	Portal* exit = nullptr;
	if (playerCollision->Intersect(bluePortal->GetComponent<CollisionComponent>()))
	{
		entry = bluePortal;
		exit = orangePortal;
	}
	if (playerCollision->Intersect(orangePortal->GetComponent<CollisionComponent>()))
	{
		entry = orangePortal;
		exit = bluePortal;
	}

	if (entry != nullptr)
	{
		PortalTeleport(entry, exit);
		mTeleportCooldown = 0.2;
		return true;
	}

	return false;
}

void PlayerMove::PortalTeleport(class Portal* entryPortal, class Portal* exitPortal)
{
	Vector3 entryFacing = entryPortal->GetNormal();
	Vector3 exitFacing = exitPortal->GetNormal();

	Vector3 facingVec = Vector3::Zero;

	// Adjust Rotation if Exit not facing Z Axis
	if (Math::Abs(exitFacing.z) != 1)
	{
		if (Math::Abs(entryFacing.z) == 1)
			facingVec = exitPortal->GetQuatForward();
		else
			facingVec = entryPortal->GetPortalOutVector(mOwner->GetForward(),exitPortal,0);

		// Calculate Angle between UnitX and Desired Facing
		facingVec.z = 0.0f;
		facingVec = Vector3::Normalize(facingVec);


		float dotProduct = Vector3::Dot(Vector3::UnitX, facingVec);
		Vector3 crossProduct = Vector3::Cross(Vector3::UnitX, facingVec);
		if (crossProduct.z > 0)
			mOwner->SetRotation(Math::Acos(dotProduct));
		else
			mOwner->SetRotation(-Math::Acos(dotProduct));
	}

	// Adjust Velocity
	float length = Math::Max(mVelocity.Vector3::Length() * 1.5f, 350.0f);
	Vector3 velForward;
	if (Math::Abs(entryFacing.z) == 1 || Math::Abs(exitFacing.z) == 1)
		velForward = Vector3::Normalize(exitPortal->GetQuatForward());
	else
		velForward = entryPortal->GetPortalOutVector(Vector3::Normalize(mVelocity), exitPortal, 0);

	mVelocity = velForward * length;

	// Adjust Position
	Vector3 position = mOwner->GetPosition();
	if (Math::Abs(entryFacing.z) == 1 || Math::Abs(exitFacing.z) == 1)
		position = exitPortal->GetPosition();
	else
		position = entryPortal->GetPortalOutVector(mOwner->GetPosition(), exitPortal, 1);

	position += exitPortal->GetQuatForward() * 50.0f;
	mOwner->SetPosition(position);

	ChangeState(MoveState::Falling);
	teleportFall = true;
	
	mOwner->GetGame()->GetAudio()->PlaySound(TELEPORT_SOUND);
}