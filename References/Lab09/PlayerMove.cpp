//Header Include
#include "PlayerMove.h"

//Components
#include "CollisionComponent.h"
#include "CameraComponent.h"

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
, mCurrentState(OnGround)
{
	mCameraComponent = owner->GetComponent<CameraComponent>();

	mCrosshair = new Crosshair(owner);
}

void PlayerMove::ProcessInput(const Uint8* keyState, Uint32 mouseButtons,
							  const Vector2& relativeMouse)
{
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

	if (keyState[SDL_SCANCODE_SPACE] && !mPreviousFrameKeyboard["SDL_SCANCODE_SPACE"])
	{
		AddForce(mJumpForce);
		ChangeState(Jump);
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
			game->GetBluePortal()->SetState(ActorState::Destroy);
			game->GetOrangePortal()->SetState(ActorState::Destroy);
			game->SetBluePortal(nullptr);
			game->SetOrangePortal(nullptr);
			mBlueExists = false;
			mOrangeExists = false;

			mCrosshair->SetState(CrosshairState::Default);
		}
	}

	mPreviousFrameKeyboard["SDL_SCANCODE_SPACE"] = keyState[SDL_SCANCODE_SPACE];
	mPreviousFrameKeyboard["SDL_SCANCODE_R"] = keyState[SDL_SCANCODE_R];
	mPreviousFrameKeyboard["SDL_BUTTON_LEFT"] = mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT);
	mPreviousFrameKeyboard["SDL_BUTTON_RIGHT"] = mouseButtons & SDL_BUTTON(SDL_BUTTON_RIGHT);

	float angularSpeed = (relativeMouse.x / MOUSE_MOVEMENT_DIVISOR) * MOUSE_MOVEMENT_MULTIPLIER;
	SetAngularSpeed(angularSpeed);

	float pitchSpeed = (relativeMouse.y / MOUSE_MOVEMENT_DIVISOR) * MOUSE_MOVEMENT_MULTIPLIER;
	mCameraComponent->SetPitchSpeed(pitchSpeed);
}

void PlayerMove::Update(float deltaTime)
{
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
}

void PlayerMove::UpdateOnGround(float deltaTime)
{
	PhysicsUpdate(deltaTime);

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
	}
}

void PlayerMove::PhysicsUpdate(float deltaTime)
{
	mAcceleration = mPendingForces * (1.0f / mMass);
	mVelocity += mAcceleration * deltaTime;
	FixXYVelocity();

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

Quaternion CreateArbitraryQuaternion(Vector3 originalFacing, Vector3 desiredFacing)
{
	Vector3 originalUnit = Vector3::Normalize(originalFacing);
	Vector3 desiredUnit = Vector3::Normalize(desiredFacing);

	float dotProduct = Vector3::Dot(originalUnit, desiredUnit);
	if (dotProduct == 1.0f)
		return Quaternion::Identity;

	Vector3 axisNormalized = Vector3::Normalize(Vector3::Cross(originalUnit, desiredUnit));
	float angle = Math::Acos(dotProduct);

	if (dotProduct == -1.0f)
	{
		axisNormalized = Vector3::UnitZ;
		angle = Math::Pi;
	}

	return Quaternion(axisNormalized, angle);
}

void PlayerMove::CreatePortal(bool isBlue)
{
	Game* game = mOwner->GetGame();

	Vector3 nearPoint = game->GetRenderer()->Unproject(Vector3(0.0f, 0.0f, 0.0f));
	Vector3 farPoint = game->GetRenderer()->Unproject(Vector3(0.0f, 0.0f, 1.0f));
	Vector3 directionVector = Vector3::Normalize(farPoint - nearPoint);

	LineSegment lineSegment = LineSegment(nearPoint, nearPoint + (directionVector * 1000.0f));
	CastInfo outInfo;

	if (SegmentCast(game->GetColliders(), lineSegment, outInfo))
	{
		Block* b = dynamic_cast<Block*>(outInfo.mActor);
		if (b != nullptr)
		{
			Portal* portal = new Portal(game, isBlue);
			portal->SetPosition(outInfo.mPoint);
			portal->SetQuat(CreateArbitraryQuaternion(Vector3::UnitX, outInfo.mNormal));

			Portal* oldPortal = nullptr;

			if (isBlue)
			{
				oldPortal = game->GetBluePortal();
				game->SetBluePortal(portal);
				mBlueExists = true;
			}
			else
			{
				oldPortal = game->GetOrangePortal();
				game->SetOrangePortal(portal);
				mOrangeExists = true;
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
		}
	}
}