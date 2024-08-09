#pragma once

#include "Actor.h"
#include <string>

class Soldier : public Actor
{
public:
	Soldier(class Game* game, class PathNode* start, class PathNode* end);
	~Soldier();

	const float SOLDIER_WIDTH = 32.0f;
	const float SOLDIER_HEIGHT = 32.0f;
	const float ANIMATION_FPS = 5.0f;
	const std::string SOLDIER_ASSETS = "Assets/Soldier";

	const int SOLDIER_HEALTH = 2;
};