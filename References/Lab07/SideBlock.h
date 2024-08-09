#pragma once

#include "Actor.h"
#include <string>

class SideBlock : public Actor
{
public:
	SideBlock(class Game* game, size_t textureIndex);
	~SideBlock();

	void OnUpdate(float deltaTime) override;

	const float SIDEBLOCK_SCALE = 500.0f;
	const std::string SIDEBLOCK_MESH = "Assets/Cube.gpmesh";

	const int DESTROY_DISTANCE = 2000;
};