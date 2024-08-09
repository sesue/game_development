#pragma once

#include "Actor.h"
#include <string>

class EnergyGlass : public Actor
{
public:
	EnergyGlass(class Game* game);
	~EnergyGlass();

	const Vector3 COLLISION_BOX = Vector3(1.0f, 1.0f, 1.0f);
	const bool ALPHA_MESH = true;
	const std::string MESH_FILE = "Assets/Meshes/Cube.gpmesh";
	const int TEXTURE_INDEX = 17;

	void OnUpdate(float deltaTime) override;
};