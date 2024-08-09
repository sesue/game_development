#pragma once

#include "Actor.h"
#include <string>

class Pellet : public Actor
{
public:
	Pellet(class Game* game, Vector3 forward);
	~Pellet();

	const Vector3 COLLISION_DIM = Vector3(25.0f, 25.0f, 25.0f);
	const std::string MESH_FILE = "Assets/Meshes/Sphere.gpmesh";
	const int INIT_TEXTURE_INDEX = 1;
	const int ENERGY_TEXTURE_INDEX = 2;

	const float SPEED = 500.0f;
	const float INIT_INVULNERABILITY = 0.25;

	const float PELLET_CATCH_DIST = 40.0f;

	const float PELLET_DAMAGE = 100.0f;

	const std::string DESTROY_SOUND = "PelletDeath.ogg";

	void OnUpdate(float deltaTime) override;

private:
	float mLifetime = 0.0f;

	Vector3 mForward;

	bool mIsEnergized = false;

	void PelletTeleport(class Portal* entryPortal, class Portal* exitPortal);
};