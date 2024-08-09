#pragma once

#include "Actor.h"
#include <string>

class Door : public Actor
{
public:
	Door(class Game* game, std::string name);
	~Door();

	std::string DOOR_MESH = "Assets/Meshes/DoorFrame.gpmesh";
	std::string DOOR_LEFT_MESH = "Assets/Meshes/DoorLeft.gpmesh";
	std::string DOOR_RIGHT_MESH = "Assets/Meshes/DoorRight.gpmesh";
	
	const std::string GetName() const { return mName; }

	void OnUpdate(float deltaTime) override;

	const Vector3 FULL_OPEN = Vector3(0.0f, 100.0f, 0.0);
	const std::string OPEN_SOUND = "DoorOpen.ogg";

	void Open();
	const bool IsOpen() { return mIsOpen; }

private:
	std::string mName;
	bool mIsOpen = false;

	Actor* mLeftHalf;
	Actor* mRightHalf;
	float mOpenTime = 0.0f;
};