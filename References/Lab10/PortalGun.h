#pragma once

#include "Actor.h"
#include <string>

class PortalGun : public Actor
{
public:
	PortalGun(class Game* game);
	~PortalGun();

	const std::string GUN_MESH = "Assets/Meshes/PortalGun.gpmesh";
	const Vector3 GUN_COLLISION_SIZE = Vector3(8.0f, 8.0f, 8.0f);

	void OnUpdate(float deltaTime) override;

private:
	class CollisionComponent* mCollision = nullptr;
};