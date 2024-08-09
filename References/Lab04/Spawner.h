#pragma once

#include "Actor.h"

class Spawner : public Actor
{
public:
	Spawner(class Game* game);
	~Spawner();

	const int SPAWN_X_DISTANCE = 600;

	void OnUpdate(float deltaTime) override;
};