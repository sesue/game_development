//Header Include
#include "EnemyMove.h"

//Actor Include
#include "Actor.h"

//Utility Includes
#include "Game.h"
#include "HeightMap.h"
#include "CSVHelper.h"
#include <fstream>

EnemyMove::EnemyMove(Actor* owner)
: VehicleMove(owner)
{
	//Load Level File
	std::ifstream file(FILE_PATH);
	HeightMap* heightMap = owner->GetGame()->GetHeightMap();

	std::string line;
	while (getline(file, line))
	{
		std::vector<std::string> point = CSVHelper::Split(line);
		if (point[0] == "Node")
			mRoute.push_back(heightMap->CellToWorld(stoi(point[1]), stoi(point[2])));
	}

	owner->SetPosition(mRoute[0]);
}

void EnemyMove::Update(float deltaTime)
{
	Vector3 position = mOwner->GetPosition();
	//Close Enough to NextPoint?
	if (Vector3::Distance(position, mRoute[mNextPoint]) < POINT_BUFFER)
		mNextPoint++;

	if (mNextPoint >= mRoute.size())
		mNextPoint = 0;

	//Is NextPoint 'reasonably' in front?
	//If so accelerate
	Vector3 toPointNormalized = Vector3::Normalize(mRoute[mNextPoint] - position);
	Vector3 forward = mOwner->GetForward();
	float dotProduct = Vector3::Dot(toPointNormalized, forward);

	if (dotProduct > DOT_PRODUCT_ACC_THRESHOLD)
		mIsAccPressed = true;
	else
		mIsAccPressed = false;

	//Do I need to turn?
	//If so which way?
	Vector3 crossProduct = Vector3::Cross(forward, toPointNormalized);
	if (dotProduct < DOT_PRODUCT_TURN_THRESHOLD)
	{
		if (crossProduct.z > 0)
			mTurn = VehicleMove::Right;
		else
			mTurn = VehicleMove::Left;
	}
	else
		mTurn = VehicleMove::None;

	VehicleMove::Update(deltaTime);
}
