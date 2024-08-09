#pragma once

#include "Actor.h"

class Log : public Actor
{
public:
	Log(class Game* game, char type, Vector2 direction);
	~Log();

	const float LOG_SPEED = 37.5f;
	const float LOG_HEIGHT = 24.0f;
	const float X_LOG_WIDTH = 96.0f;
	const float Y_LOG_WIDTH = 128.0f;
	const float Z_LOG_WIDTH = 192.0f;

	class CollisionComponent* GetCollision() { return mCollision; }
	class WrappingMove* GetMove() { return mMove; }

private:
	class SpriteComponent* mSprite = nullptr;
	class WrappingMove* mMove = nullptr;
	class CollisionComponent* mCollision = nullptr;
	const int DRAW_LAYER = 1;
};