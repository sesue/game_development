#pragma once

#include "Actor.h"
#include <string>

class Block : public Actor
{
public:
	Block(class Game* game);
	~Block();

	const float BLOCK_SCALE = 64.0f;
	const Vector3 BLOCK_COLLISION_SIZE = Vector3(1.0f, 1.0f, 1.0f);
	const std::string BLOCK_MESH = "Assets/Meshes/Cube.gpmesh";
};