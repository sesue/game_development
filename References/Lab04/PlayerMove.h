#pragma once
#include "MoveComponent.h"
#include "Math.h"

class PlayerMove : public MoveComponent
{
public:
	PlayerMove(class Actor* owner);

	const Vector2 CAMERA_OFFSET = Vector2(-256.0f, -224.0f);
	const float INITIAL_HORITZONTAL_SPEED = 300.0f;
	const float INITIAL_JUMP_SPEED = -700.0f;

	// Update the move component
	void ProcessInput(const Uint8* keyState) override;
	void Update(float deltaTime) override;
	void UpdateAnimation(float deltaTime);
	void KillPlayer();
	void PlayerWin();

private:
	float mXSpeed = 0;
	float mYSpeed = 0;
	bool mSpacePressed = false;
	bool mInAir = false;
};
