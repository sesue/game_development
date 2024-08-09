#pragma once

#include "Actor.h"

class DeadFrog : public Actor
{
public:
	DeadFrog(class Game* game);

	void OnUpdate(float deltaTime) override;

private:
	class SpriteComponent* mSprite = nullptr;
	float mLifeTime = 0.5f;
};