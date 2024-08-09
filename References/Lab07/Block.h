#pragma once

#include "Actor.h"
#include <string>

class Block : public Actor
{
public:
	Block(class Game* game, bool isExploding);
	~Block();

	void OnUpdate(float deltaTime) override;

	bool Explode();

	const float BLOCK_SCALE = 25.0f;
	const std::string BLOCK_MESH = "Assets/Cube.gpmesh";
	const int REGULAR_TEXTURE_INDEX = 3;
	const int EXPLODING_TEXTURE_INDEX = 4;

	const Vector3 COLLISION_BOX = Vector3(1.0f, 1.0f, 1.0f);

	const int DESTROY_DISTANCE = 2000;

	const int EXPLOSION_DISTANCE = 50;

private:
	bool mIsExploding;
};