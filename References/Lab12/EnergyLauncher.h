#pragma once

#include "Actor.h"
#include <string>

class EnergyLauncher : public Actor
{
public:
	EnergyLauncher(class Game* game, float cooldown, std::string door);
	~EnergyLauncher();

	const float COLLISION_SIZE = 50.0f;
	const std::string ENERGY_LAUNCHER_MESH = "Assets/Meshes/EnergyLauncher.gpmesh";

	void OnUpdate(float deltaTime) override;

	const std::string FIRE_SOUND = "PelletFire.ogg";

private:
	float mCooldown = 1.5f;
	float mTimer = 0.0f;

	std::string mDoor;
	bool mIsActive = true;
};