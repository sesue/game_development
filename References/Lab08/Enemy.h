#pragma once

#include "Actor.h"
#include <string>

class Enemy : public Actor
{
public:
	Enemy(class Game* game);
	~Enemy();

	const Vector3 ENEMY_COLLISION_SIZE = Vector3(40.0f, 25.0f, 15.0f);
	const float ENEMY_SCALE = 0.75f;
	const std::string ENEMY_MESH = "Assets/Kart.gpmesh";
	const int ENEMY_TEXTURE_INDEX = 6;

private:
};