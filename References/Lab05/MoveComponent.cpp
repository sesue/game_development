#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner)
: Component(owner, 50)
, mAngularSpeed(0.0f)
, mForwardSpeed(0.0f)
{
}

void MoveComponent::Update(float deltaTime)
{
	//Angular Movement (radians/second)
	mOwner->SetRotation(mOwner->GetRotation() + (mAngularSpeed * deltaTime));

	//Translating Movement (pixels/second)
	Vector2 velocity = mOwner->GetForward() * GetForwardSpeed();
	mOwner->SetPosition(mOwner->GetPosition() + (velocity * deltaTime));
}
