#pragma once
#include "MoveComponent.h"
#include "Math.h"
#include <map>
#include "SDL2/SDL_scancode.h"
#include "AudioSystem.h"

class PlayerMove : public MoveComponent
{
public:
	PlayerMove(class Actor* owner);

	//Movement Constants
	const float X_SPEED = 400.0f;
	const float Y_SPEED = 300.0f;
	const float Z_SPEED = 300.0f;

	const float SPEED_MULTIPLIER_INCREMENT = 0.15f;
	const float SPEED_MULTIPLIER_TIME = 10.0f;

	const float UPPER_Y = 180;
	const float LOWER_Y = -180;
	const float UPPER_Z = 225;
	const float LOWER_Z = -225;

	//Camera Constants
	const float HORIZONTAL_DISTANCE = 300.0f;
	const float VERTICAL_DISTANCE = 0.0f;
	const float TARGET_DISTANCE = 20.0f;

	//Side Block Constants
	const int SIDEBLOCK_REPEAT_DISTANCE = 500;
	const int SIDEBLOCK_END_DISTANCE = 4000;

	const Vector3 RIGHT_POSITION = Vector3(0.0f, 500.0f, 0.0f);
	const Vector3 LEFT_POSITION = Vector3(0.0f, -500.0f, 0.0f);
	const Vector3 TOP_POSITION = Vector3(0.0f, 0.0f, 500.0f);
	const Vector3 BOTTOM_POSITION = Vector3(0.0f, 0.0f, -500.0f);

	const int SIDE_INDECIES[4] = {0, 1, 2, 0};
	const int TOP_INDECIES[2] = {6, 7};
	const int BOTTOM_INDEX = 5;

	//Shield Constants
	const int MAX_SHIELD = 3;
	const float INVICIBILITY_TIME = 1.0f;
	const int DAMAGE_PER_HIT = 1;

	const float BARREL_ROLL_TIME = 0.5;
	const float ROLL_ANGULAR_SPEED = 8.0f * Math::Pi;
	const float BARREL_ROLL_VOICE_MAX_TIME = 25.0;
	const float BARREL_ROLL_VOICE_MIN_TIME = 15.0;

	// Update the move component
	void ProcessInput(const Uint8* keyState) override;
	void Update(float deltaTime) override;

	// Getters/setters
	const float& GetSpeedMultiplier() const { return mSpeedMultiplier; }
	const int& GetShield() const { return mShield; }

protected:
	Vector3 mVelocity = Vector3::Zero;
	float mSpeedMultiplier = 1.0;
	float mSpeedTimer = SPEED_MULTIPLIER_TIME;

	std::map<SDL_Scancode, bool> mPreviousFrameKeyboard = {{SDL_SCANCODE_SPACE, false},
														   {SDL_SCANCODE_Q, false}};

	float mInvicibility = 0.0f;
	int mShield = MAX_SHIELD;

	float mBarrelRollTimer = 0.0f;
	float mDoABarrelRollVoiceLine = 0.0f;

	//Side Block Function and Variables
	void AddSideBlocks(int x);
	int mNextBlockX = 0;

	//Helper Functions
	void CheckForCollisions(float deltaTime);

	void AdjustShield(bool hurt);

	//Sound Handlers
	SoundHandle mShipLoop = SoundHandle::Invalid;
	SoundHandle mDamageAlert = SoundHandle::Invalid;
};
