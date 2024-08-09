#pragma once

#include "Actor.h"
#include <string>

class EnergyCube : public Actor
{
public:
	EnergyCube(class Game* game);
	~EnergyCube();

	const Vector3 COLLISION_BOX = Vector3(25.0f, 25.0f, 25.0f);
	const bool ALPHA_MESH = false;
	const std::string MESH_FILE = "Assets/Meshes/EnergyCube.gpmesh";

	void OnUpdate(float deltaTime) override;
};