#pragma once

#include "Actor.h"
#include "SDL2/SDL.h"

class Asteroid : public Actor
{
public:
	Asteroid(class Game* game);

private:
	~Asteroid();
	void OnUpdate(float deltaTime) override;

	class MoveComponent* mMovement = nullptr;
	class SpriteComponent* mSprite = nullptr;
	const float FORWARD_SPEED = 150.0f;
	const int DRAW_LAYER = 2;
};