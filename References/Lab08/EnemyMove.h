#pragma once
#include "VehicleMove.h"
#include <string>
#include <vector>

class EnemyMove : public VehicleMove
{
public:
	EnemyMove(class Actor* owner);

	// Update the move component
	void Update(float deltaTime) override;

	const std::string FILE_PATH = "Assets/HeightMap/Path.csv";

	const float POINT_BUFFER = 100.0f;
	const float DOT_PRODUCT_ACC_THRESHOLD = 0.9;
	const float DOT_PRODUCT_TURN_THRESHOLD = 1;

private:
	std::vector<Vector3> mRoute;
	int mNextPoint = 1;
};
