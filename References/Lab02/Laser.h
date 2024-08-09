#pragma once

#include "Actor.h"
#include "SDL2/SDL.h"

class Laser : public Actor
{
public:
	Laser(class Game* game);

protected:
	class MoveComponent* mMovement = nullptr;
	class SpriteComponent* mSprite = nullptr;
	const float FORWARD_SPEED = 400.0f;
	const float HIT_DISTANCE = 70.0f;
	const int DRAW_LAYER = 3;

	float mLifetime = 1.0f;

	void OnUpdate(float deltaTime) override;
};