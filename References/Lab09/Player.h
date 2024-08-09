#pragma once

#include "Actor.h"

class Player : public Actor
{
public:
	Player(class Game* game, Vector3 initialPos);
	~Player();

	const Vector3 PLAYER_COLLISION_SIZE = Vector3(50.0f, 100.0f, 50.0f);

	const bool HasGun() const { return mHasGun; }
	void GiveGun();

	Vector3 mInitialPos;

private:
	bool mHasGun = false;
};