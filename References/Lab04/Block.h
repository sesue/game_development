#pragma once

#include "Actor.h"

class Block : public Actor
{
public:
	Block(class Game* game, char type);
	~Block();

	const float BLOCK_HEIGHT = 32.0f;
	const float BLOCK_WIDTH = 32.0f;

	const class CollisionComponent* GetCollision() { return mCollision; }

private:
	class SpriteComponent* mSprite = nullptr;
	class CollisionComponent* mCollision = nullptr;
	const int DRAW_LAYER = 1;
};