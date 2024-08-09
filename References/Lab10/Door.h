#pragma once

#include "Actor.h"
#include <string>

class Door : public Actor
{
public:
	Door(class Game* game, std::string name);
	~Door();

	std::string DOOR_MESH = "Assets/Meshes/DoorFrame.gpmesh";
	
	const std::string GetName() const { return mName; }

	void Open();
	const bool IsOpen() { return mIsOpen; }

private:
	std::string mName;
	bool mIsOpen = false;
};