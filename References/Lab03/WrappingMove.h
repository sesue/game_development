#pragma once

#include "MoveComponent.h"
#include "Math.h"

class WrappingMove : public MoveComponent
{
public:
	WrappingMove(class Actor* owner, float speed, Vector2 direction);

	// Update the move component
	void Update(float deltaTime) override;

	Vector2 mDirection = Vector2(0, 0);
};