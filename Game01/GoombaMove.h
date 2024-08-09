#pragma once

#include "MoveComponent.h"
#include "Math.h"

class GoombaMove : public MoveComponent
{
public:
	GoombaMove(class Goomba* owner, class CollisionComponent* collision);
	~GoombaMove();

	void Update(float deltaTime) override;

private:
	Vector2 mVelocity = Vector2(0.0f, 0.0f);
	class CollisionComponent* mOwnerCollision = nullptr;
};