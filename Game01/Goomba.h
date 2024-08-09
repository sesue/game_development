#pragma once

#include "Actor.h"

class Goomba : public Actor
{
public:
	Goomba(class Game* game);
	~Goomba();

	void OnUpdate(float deltaTime) override;
	void Stomp();

	class CollisionComponent* GetCollision() const { return mCollision; }
	bool GetStomped() const { return mStomped; }
	void SetStomped(const bool& isStomped) { mStomped = isStomped; }

	const int DRAW_LAYER = 150;
	const float GOOMBA_HEIGHT = 32.0f;
	const float GOOMBA_WIDTH = 32.0f;

private:
	class AnimatedSprite* mSprite = nullptr;
	class CollisionComponent* mCollision = nullptr;
	class GoombaMove* mMove = nullptr;

	bool mStomped = false;
	float mLifeTime = 0.25f;
};