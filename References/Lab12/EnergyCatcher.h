#pragma once

#include "Actor.h"
#include <string>

class EnergyCatcher : public Actor
{
public:
	EnergyCatcher(class Game* game, std::string door);
	~EnergyCatcher();

	const float COLLISION_SIZE = 50.0f;
	const std::string ENERGYCATCHER_MESH = "Assets/Meshes/EnergyCatcher.gpmesh";

	const bool IsActivated() const { return mActivated; }
	void Activate();

	const std::string ACTIVATE_SOUND = "EnergyCaught.ogg";

private:
	std::string mDoor;
	bool mActivated = false;
};