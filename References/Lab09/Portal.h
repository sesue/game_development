#pragma once
#include "Actor.h"
#include <string>

class Portal : public Actor
{
public:
	Portal(class Game* game, bool isBlue);

	const std::string PORTAL_MESH = "Assets/Meshes/Portal.gpmesh";
};
