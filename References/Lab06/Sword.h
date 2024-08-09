#pragma once

#include "Actor.h"

class Sword : public Actor
{
public:
	Sword(class Game* game);
	~Sword();

	const float SWORD_WIDTH = 28.0f;
	const float SWORD_HEIGHT = 28.0f;
};