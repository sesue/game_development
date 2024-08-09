#include "WrappingMove.h"
#include "Actor.h"
#include "Game.h"

WrappingMove::WrappingMove(class Actor* owner, float speed, Vector2 direction)
: MoveComponent(owner, speed)
, mDirection(direction)
{
}

void WrappingMove::Update(float deltaTime)
{
	//Translating Movement (pixels/second)
	Vector2 velocity = Vector2(mDirection.x * GetForwardSpeed(), mDirection.y);
	mOwner->SetPosition(mOwner->GetPosition() + (velocity * deltaTime));

	if (mOwner->GetPosition().x < 0)
		mOwner->SetPosition(
			Vector2(static_cast<float>(GetGame()->WINDOW_WIDTH), mOwner->GetPosition().y));
	else if (mOwner->GetPosition().x > GetGame()->WINDOW_WIDTH)
		mOwner->SetPosition(Vector2(0.0f, mOwner->GetPosition().y));
}