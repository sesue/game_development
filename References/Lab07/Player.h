#pragma once

#include "Actor.h"
#include <string>

class Player : public Actor
{
public:
	Player(class Game* game);
	~Player();

	const Vector3 PLAYER_COLLISION_SIZE = Vector3(40.0f, 25.0f, 15.0f);
	const float PLAYER_SCALE = 2.0f;
	const std::string PLAYER_MESH = "Assets/Arwing.gpmesh";

	const class CollisionComponent* GetCollision() { return mCollision; }

private:
	class CollisionComponent* mCollision = nullptr;
};