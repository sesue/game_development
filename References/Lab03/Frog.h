#pragma once

#include "Actor.h"
#include "SDL2/SDL_scancode.h"
#include <map>
#include <string>

class Frog : public Actor
{
public:
	Frog(class Game* game, const Vector2 startPosition);

	const float FROG_COLLISION_WIDTH = 25.0f;
	const float FROG_COLLISION_HEIGHT = 25.0f;

private:
	class SpriteComponent* mSprite = nullptr;
	class CollisionComponent* mCollision = nullptr;
	const int DRAW_LAYER = 1;

	Vector2 mStartPosition = Vector2(0.0f, 0.0f);

	//Game Logic Variables
	std::map<SDL_Scancode, bool> mPreviousFrameKeyboard = {{SDL_SCANCODE_W, false},
														   {SDL_SCANCODE_S, false},
														   {SDL_SCANCODE_A, false},
														   {SDL_SCANCODE_D, false}};
	const float STEP_DISTANCE = 32.0f;

	void OnProcessInput(const Uint8* keyState) override;
	void OnUpdate(float deltaTime) override;
};