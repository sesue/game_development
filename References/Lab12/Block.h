#pragma once

#include "Actor.h"
#include <string>

class Block : public Actor
{
public:
	Block(class Game* game);
	~Block();

	const int BLOCK_SCALE = 64;
	const Vector3 BLOCK_COLLISION_SIZE = Vector3(1.0f, 1.0f, 1.0f);
	std::string BLOCK_MESH = "Assets/Meshes/Cube.gpmesh";
};