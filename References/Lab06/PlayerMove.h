#pragma once
#include "MoveComponent.h"
#include "Math.h"

class PlayerMove : public MoveComponent
{
public:
	enum Facing
	{
		Up,
		Right,
		Down,
		Left
	};

	PlayerMove(class Actor* owner);

	const float MOVEMENT_SPEED = 150.0f;
	const float ATTACK_TIME = 0.25;

	// Update the move component
	void ProcessInput(const Uint8* keyState) override;
	void Update(float deltaTime) override;
	void UpdateAnimation(float deltaTime);

	void UpdateSword();

private:
	Vector2 mDirection = Vector2(0.0f, 0.0f);
	Facing mFacing = Down;
	bool mMoving = false;

	class Sword* mSword = nullptr;
	class CollisionComponent* mSwordCollision = nullptr;

	class AnimatedSprite* mSprite = nullptr;
	class CollisionComponent* mPlayerCollision = nullptr;

	float mAttackLifeTime = 0.0f;
	bool mSpacePressedPrevFrame = false;
};
