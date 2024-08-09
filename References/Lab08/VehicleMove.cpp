//Header Include
#include "VehicleMove.h"

//Actor Include
#include "Actor.h"

//Utility Includes
#include "Game.h"
#include "HeightMap.h"
#include "CSVHelper.h"
#include <fstream>

VehicleMove::VehicleMove(Actor* owner)
: Component(owner, 50.0f)
{
	//Load Level File
	std::ifstream file(FILE_PATH);

	std::string line;
	while (getline(file, line))
	{
		std::vector<std::string> checkpoint = CSVHelper::Split(line);
		if (checkpoint[0] == "Checkpoint")
		{
			Vector2 minCell = Vector2(stoi(checkpoint[1]), stoi(checkpoint[3]));
			Vector2 maxCell = Vector2(stoi(checkpoint[2]), stoi(checkpoint[4]));
			mCheckPoints.push_back(std::make_pair(minCell, maxCell));
		}
	}

	mHeightMap = owner->GetGame()->GetHeightMap();
}

bool static InBetween(Vector2 pos, Vector2 min, Vector2 max)
{
	int posX = static_cast<int>(pos.x);
	int posY = static_cast<int>(pos.y);

	if (posX >= min.x && pos.x <= max.x)
		if (posY >= min.y && pos.y <= max.y)
			return true;

	return false;
}

void VehicleMove::Update(float deltaTime)
{
	if (mIsAccPressed)
	{
		if (mAccTime < ACC_RAMP_TIME)
			mAccTime += deltaTime;

		float accelerationMagnitude = Math::Lerp(MIN_LINEAR_ACC_MAGNITUDE, MAX_LINEAR_ACC_MAGNITUDE,
												 mAccTime / ACC_RAMP_TIME);
		mVelocity += mOwner->GetForward() * accelerationMagnitude * deltaTime;
	}
	else
		mAccTime = 0.0f;

	Vector3 position = mOwner->GetPosition();
	position = position + (mVelocity * deltaTime);
	if (mHeightMap->IsOnTrack(Vector2(position.x, position.y)))
		position.z = Math::Lerp(position.z, mHeightMap->GetHeight(Vector2(position.x, position.y)),
								LERP_CONSTANT);
	mOwner->SetPosition(position);

	if (mIsAccPressed)
		mVelocity *= LINEAR_DRAG_COEFFICIENT_ACC;
	else
		mVelocity *= LINEAR_DRAG_COEFFICIENT;

	if (mTurn == Left)
		mAngularVel -= ANGULAR_ACC * deltaTime;
	else if (mTurn == Right)
		mAngularVel += ANGULAR_ACC * deltaTime;
	else
		mAngularVel = 0.0f;

	mOwner->SetRotation(mOwner->GetRotation() + mAngularVel * deltaTime);
	mAngularVel *= ANGULAR_DRAG_COEFFICIENT;

	//Checkpoint Check
	Vector2 cell = mHeightMap->WorldToCell(Vector2(position.x, position.y));
	if (InBetween(cell, mCheckPoints[mLastCheckpointInd + 1].first,
				  mCheckPoints[mLastCheckpointInd + 1].second))
	{
		int maxIndex = mCheckPoints.size() - 2;
		if (mLastCheckpointInd < maxIndex)
			mLastCheckpointInd++;
		else
			mLastCheckpointInd = -1;

		if (mLastCheckpointInd == 0)
		{
			mLap++;
			OnLapChange(mLap);
		}
	}
}

Vector2 GetCheckpointCenter(Vector2 min, Vector2 max)
{
	float xDiff = max.x - min.x;
	float yDiff = max.y - min.y;

	return Vector2(min.x + (xDiff / 2.0f), min.y + (yDiff / 2.0f));
}

float VehicleMove::GetDistToNextCheckpoint()
{
	Vector3 worldPosition = mOwner->GetPosition();
	Vector2 position = mHeightMap->WorldToCell(Vector2(worldPosition.x, worldPosition.y));

	Vector2 min = mCheckPoints[mLastCheckpointInd + 1].first;
	Vector2 max = mCheckPoints[mLastCheckpointInd + 1].second;
	Vector2 center = GetCheckpointCenter(min, max);

	return Vector2::Distance(position, center);
}
