#pragma once

#include "Actor.h"
#include "SDL2/SDL.h"

class Ship : public Actor
{
public:
	Ship(class Game* game);

private:
	class MoveComponent* mMovement = nullptr;
	class SpriteComponent* mSprite = nullptr;
	const float MAX_FORWARD_SPEED = 400;
	const float MAX_ANGULAR_SPEED = Math::TwoPi;
	const int DRAW_LAYER = 1;

	float mLaserCooldown = 0.0f;

	void OnProcessInput(const Uint8* keyState) override;
	void OnUpdate(float deltaTime) override;
};