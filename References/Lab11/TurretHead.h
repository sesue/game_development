#pragma once

#include "Actor.h"
#include <string>
#include "SDL2/SDL.h"

enum class TurretState
{
	Idle,
	Search,
	Priming,
	Firing,
	Falling,
	Dead
};

class TurretHead : public Actor
{
public:
	TurretHead(class Game* game, Actor* parent);
	~TurretHead();

	// Initializing Constants
	const float INIT_SCALE = 0.75f;
	const Vector3 INIT_POSITION = Vector3(0.0f, 0.0f, 18.75f);
	const std::string MESH_FILE = "Assets/Meshes/TurretHead.gpmesh";
	const int INIT_TEXTURE_INDEX = 1;

	void OnUpdate(float deltaTime) override;

	// Update Constants
	const float ROTATION_SPEED = Math::Pi;

	const Vector3 LASER_POS = Vector3(0.0f, 0.0f, 10.0f);

	// State Constants
	const float PRIMING_TIMEOUT = 1.5f;
	const float SEARCH_TIMEOUT = 5.0f;

	const float UP_DIST = 25.0f;
	const float SIDE_DIST = 75.0f;
	const float FWD_DIST = 200.0f;
	const float MOVE_TIME = 0.5;

	const float PORTAL_COOLDOWN = 0.25f;
	const float EXIT_MAGNITUDE = 250.0f;
	const Vector3 GRAVITY = Vector3(0.0f, 0.0f, -980.0f);
	const float TERMINAL_VELOCITY = 800.0f;
	const float LANDING_CORRECTION = 15.0f;

	const float FIRING_COOLDOWN = 0.05f;
	const float TURRET_DAMAGE = 2.5f;

	void Die();

private:
	Actor* mLaserActor;
	class LaserComponent* mLaser;

	//State Machine Variables
	TurretState mCurrState = TurretState::Idle;
	float mStateTimer = 0.0f;

	void ChangeState(TurretState state)
	{
		mCurrState = state;
		mStateTimer = 0.0f;
		mFireTimer = 0.0f;
	};

	void UpdateIdle(float deltaTime);
	void UpdateSearch(float deltaTime);
	void UpdatePriming(float deltaTime);
	void UpdateFiring(float deltaTime);
	void UpdateFalling(float deltaTime);
	void UpdateDead(float deltaTime);

	void CheckForTarget();
	Actor* mTarget = nullptr;
	Quaternion mEndQuat = Quaternion::Identity;
	Quaternion mStartQuat = Quaternion::Identity;
	float mMoveTimer = 0.0f;

	bool CheckForTeleport();
	Vector3 mVelocity = Vector3::Zero;
	float mTeleportTimer = 0.0f;

	float mFireTimer = 0.0f;
};