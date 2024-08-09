#pragma once
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include <map>
#include <string>

class PlayerMove : public MoveComponent
{
public:
	enum MoveState
	{
		OnGround,
		Jump,
		Falling
	};

	PlayerMove(class Actor* owner);

	// Update the move component
	void ProcessInput(const Uint8* keyState, Uint32 mouseButtons,
					  const Vector2& relativeMouse) override;
	void Update(float deltaTime) override;

	const Vector3& GetVelocity() const { return mVelocity; }
	const Vector3& GetAcceleration() const { return mAcceleration; }

	const float MOVEMENT_SCALE = 700.0f;
	const float MAX_SPEED = 400.0f;
	const float BRAKING_FACTOR = 0.9f;

	const float RELOAD_FLOOR = -750.0f;

protected:
	std::map<std::string, bool> mPreviousFrameKeyboard = {{"SDL_SCANCODE_SPACE", false},
														  {"SDL_SCANCODE_R", false},
														  {"SDL_BUTTON_LEFT", false},
														  {"SDL_BUTTON_RIGHT", false}};

	class CameraComponent* mCameraComponent = nullptr;

	// Forces and Physics
	Vector3 mVelocity;
	Vector3 mAcceleration;
	Vector3 mGravity = Vector3(0.0f, 0.0f, -980.0f);
	Vector3 mJumpForce = Vector3(0.0f, 0.0f, 35000.0f);
	Vector3 mPendingForces;
	float mMass = 1.0f;

	void FixXYVelocity();

	void PhysicsUpdate(float deltaTime);
	void AddForce(const Vector3& force) { mPendingForces += force; }

	// State Variable and Functions
	MoveState mCurrentState;
	void ChangeState(MoveState moveState) { mCurrentState = moveState; }

	// Update Functions
	void UpdateOnGround(float deltaTime);
	void UpdateJump(float deltaTime);
	void UpdateFalling(float deltaTime);

	// Collision Functions
	CollSide FixCollision(CollisionComponent* self, CollisionComponent* collider);

	//Portal Fun Related Variables and Functions
	class Crosshair* mCrosshair = nullptr;
	bool mBlueExists = false;
	bool mOrangeExists = false;

	void CreatePortal(bool isBlue);

	bool UpdatePortalTeleport(float deltaTime);
	void PortalTeleport(class Portal* entryPortal, class Portal* exitPortal);
	float mTeleportCooldown = 0.0f;
	bool teleportFall = false;
};
