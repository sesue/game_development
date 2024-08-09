#include "CollisionComponent.h"
#include "Actor.h"
#include <algorithm>

CollisionComponent::CollisionComponent(class Actor* owner)
: Component(owner)
, mWidth(0.0f)
, mHeight(0.0f)
{
}

CollisionComponent::~CollisionComponent()
{
}

bool CollisionComponent::Intersect(const CollisionComponent* other) const
{
	bool noIntersection = GetMax().x < other->GetMin().x || other->GetMax().x < GetMin().x ||
						  GetMax().y < other->GetMin().y || other->GetMax().y < GetMin().y;
	return !noIntersection;
}

Vector2 CollisionComponent::GetMin() const
{
	Vector2 min(0, 0);
	min.x = mOwner->GetPosition().x - (mWidth * mOwner->GetScale()) / 2.0f;
	min.y = mOwner->GetPosition().y - (mHeight * mOwner->GetScale()) / 2.0f;
	return min;
}

Vector2 CollisionComponent::GetMax() const
{
	Vector2 min(0, 0);
	min.x = mOwner->GetPosition().x + (mWidth * mOwner->GetScale()) / 2.0f;
	min.y = mOwner->GetPosition().y + (mHeight * mOwner->GetScale()) / 2.0f;
	return min;
}

const Vector2& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

float Min(float a, float b, float c, float d)
{
	float minE = Math::Min(a, b);
	float minG = Math::Min(c, d);
	return Math::Min(minE, minG);
}

CollSide CollisionComponent::GetMinOverlap(const CollisionComponent* other, Vector2& offset) const
{
	offset = Vector2::Zero;

	if (Intersect(other))
	{
		float topDist = Math::Abs(other->GetMin().y - GetMax().y);
		float bottomDist = Math::Abs(other->GetMax().y - GetMin().y);
		float leftDist = Math::Abs(other->GetMin().x - GetMax().x);
		float rightDist = Math::Abs(other->GetMax().x - GetMin().x);
		float minDist = Min(topDist, bottomDist, leftDist, rightDist);

		if (minDist == topDist)
		{
			offset.y -= topDist;
			return CollSide::Top;
		}
		else if (minDist == bottomDist)
		{
			offset.y += bottomDist;
			return CollSide::Bottom;
		}
		else if (minDist == leftDist)
		{
			offset.x -= leftDist;
			return CollSide::Left;
		}
		else
		{
			offset.x += rightDist;
			return CollSide::Right;
		}
	}

	return CollSide::None;
}