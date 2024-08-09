#pragma once

#include "Actor.h"
#include <string>

class Bush : public Actor
{
public:
	Bush(class Game* game);
	~Bush();

	const float BUSH_WIDTH = 32.0f;
	const float BUSH_HEIGHT = 32.0f;
	const std::string BUSH_ASSET = "Assets/Bush.png";

	const int BUSH_HEALTH = 1;
};