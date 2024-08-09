#pragma once

#include "Actor.h"

class Vehicle : public Actor
{
public:
	Vehicle(class Game* game, char type, Vector2 direction);
	~Vehicle();

	class CollisionComponent* GetCollision() { return mCollision; }

	const float VEHICLE_SPEED = 50.0f;
	const float VEHICLE_SLOW_SPEED = 30.0f;
	const float CAR_COLLISION_WIDTH = 32.0f;
	const float CAR_COLLISION_HEIGHT = 32.0f;
	const float TRUCK_COLLISION_WIDTH = 64.0f;
	const float TRUCK_COLLISION_HEIGHT = 24.0f;

private:
	class SpriteComponent* mSprite = nullptr;
	class WrappingMove* mMove = nullptr;
	class CollisionComponent* mCollision = nullptr;
	const int DRAW_LAYER = 1;
	const float ANGLE_OF_VISION = (Math::Pi / 6.0f);

	void OnUpdate(float deltaTime) override;
};