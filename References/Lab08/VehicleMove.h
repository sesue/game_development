#pragma once
#include "Component.h"
#include "Math.h"
#include <vector>
#include <string>
#include <utility>

class VehicleMove : public Component
{
public:
	enum Turning
	{
		Left,
		Right,
		None
	};

	VehicleMove(class Actor* owner);

	//Constants
	const float MIN_LINEAR_ACC_MAGNITUDE = 1000.0f;
	const float MAX_LINEAR_ACC_MAGNITUDE = 2500.0f;
	const float ACC_RAMP_TIME = 2.0f;
	const float ANGULAR_ACC = 5.0F * Math::Pi;
	const float LINEAR_DRAG_COEFFICIENT = 0.975f;
	const float LINEAR_DRAG_COEFFICIENT_ACC = 0.9f;
	const float ANGULAR_DRAG_COEFFICIENT = 0.9f;

	const float LERP_CONSTANT = 0.1f;

	const std::string FILE_PATH = "Assets/HeightMap/Checkpoints.csv";

	// Update the move component
	void Update(float deltaTime) override;
	virtual void OnLapChange(int newLap) {}

	// Getters and Setters
	void SetAccPressed(bool pressed) { mIsAccPressed = pressed; }
	void SetTurn(Turning turn) { mTurn = turn; }
	float GetDistToNextCheckpoint();
	const int GetLap() const { return mLap; }
	const int GetNextCheckpoint() const { return mLastCheckpointInd + 1; }

protected:
	Turning mTurn = None;

	bool mIsAccPressed = false;
	float mAccTime = 0.0f;

	Vector3 mVelocity = Vector3::Zero;
	float mAngularVel = 0.0f;

	class HeightMap* mHeightMap = nullptr;

	std::vector<std::pair<Vector2, Vector2>> mCheckPoints;
	int mLap = 0;
	int mLastCheckpointInd = -1;
};
