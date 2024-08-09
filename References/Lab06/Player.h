#pragma once

#include "Actor.h"
#include <string>

class Player : public Actor
{
public:
	Player(class Game* game);
	~Player();

	const float PLAYER_HEIGHT = 20.0f;
	const float PLAYER_WIDTH = 20.0f;

	class CollisionComponent* GetCollision() const { return mCollision; }
	class AnimatedSprite* GetSprite() { return mSprite; }

private:
	class AnimatedSprite* mSprite = nullptr;
	class CollisionComponent* mCollision = nullptr;
	class PlayerMove* mMove = nullptr;
	const int DRAW_LAYER = 200;
};