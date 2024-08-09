#pragma once

#include "Actor.h"
#include "TurretHead.h"
#include <string>

class TurretBase : public Actor
{
public:
	TurretBase(class Game* game);
	~TurretBase();

	// Initializing Constants
	const float INIT_SCALE = 0.75f;
	const Vector3 INIT_COLLISION = Vector3(25.0f, 110.0f, 25.0f);
	const std::string MESH_FILE = "Assets/Meshes/TurretBase.gpmesh";
	const int INIT_TEXTURE_INDEX = 1;

	void OnUpdate(float deltaTime) override;

	void Die() { mHead->Die(); }
	

private:
	TurretHead* mHead = nullptr;
};