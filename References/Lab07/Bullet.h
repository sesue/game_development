#pragma once

#include "Actor.h"
#include <string>

class Bullet : public Actor
{
public:
	Bullet(class Game* game, Vector3 position);
	~Bullet();

	void OnUpdate(float deltaTime) override;

	const float BULLET_SCALE = 5.0f;
	const std::string BULLET_MESH = "Assets/Laser.gpmesh";
	const Vector3 COLLISION_BOX = Vector3(10, 10, 10);
	const float INITIAL_SPEED = 900.0f;

private:
	float mLifeTime = 1.0f;
};