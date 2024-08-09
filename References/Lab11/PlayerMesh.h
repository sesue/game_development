#pragma once

#include "Actor.h"
#include <string>

class PlayerMesh : public Actor
{
public:
	PlayerMesh(class Game* game);
	~PlayerMesh();

	const Vector3 PLAYER_MESH_SCALE = Vector3(1.0f, 2.5f, 2.5f);
	const std::string PLAYER_MESH = "Assets/Meshes/PortalGun.gpmesh";

	void OnUpdate(float deltaTime) override;
};