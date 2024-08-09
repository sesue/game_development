#include "CollisionComponent.h"
#include "Actor.h"
#include <algorithm>

CollisionComponent::CollisionComponent(class Actor* owner)
: Component(owner)
, mWidth(0.0f)
, mHeight(0.0f)
, mDepth(0.0f)
{
}

CollisionComponent::~CollisionComponent()
{
}

bool CollisionComponent::Intersect(const CollisionComponent* other) const
{
	Vector3 thisMin = GetMin();
	Vector3 thisMax = GetMax();
	Vector3 otherMin = other->GetMin();
	Vector3 otherMax = other->GetMax();

	bool noIntersection = thisMax.x < otherMin.x || thisMax.y < otherMin.y ||
						  thisMax.z < otherMin.z || otherMax.x < thisMin.x ||
						  otherMax.y < thisMin.y || otherMax.z < thisMin.z;

	return !noIntersection;
}

Vector3 CollisionComponent::GetMin() const
{
	Vector3 v = mOwner->GetPosition();
	v.x -= mDepth * mOwner->GetScale().x / 2.0f;
	v.y -= mWidth * mOwner->GetScale().y / 2.0f;
	v.z -= mHeight * mOwner->GetScale().z / 2.0f;
	return v;
}

Vector3 CollisionComponent::GetMax() const
{
	Vector3 v = mOwner->GetPosition();
	v.x += mDepth * mOwner->GetScale().x / 2.0f;
	v.y += mWidth * mOwner->GetScale().y / 2.0f;
	v.z += mHeight * mOwner->GetScale().z / 2.0f;
	return v;
}

Vector3 CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

float Min(float a, float b, float c, float d, float e, float f)
{
	float min1 = Math::Min(a, b);
	float min2 = Math::Min(c, d);
	float min3 = Math::Min(e, f);
	min2 = Math::Min(min2, min3);
	return Math::Min(min1, min2);
}

CollSide CollisionComponent::GetMinOverlap(const CollisionComponent* other, Vector3& offset) const
{
	offset = Vector3::Zero;

	if (Intersect(other))
	{
		float topDist = Math::Abs(other->GetMax().z - GetMin().z);
		float bottomDist = Math::Abs(other->GetMin().z - GetMax().z);
		float leftDist = Math::Abs(other->GetMin().y - GetMax().y);
		float rightDist = Math::Abs(other->GetMax().y - GetMin().y);
		float frontDist = Math::Abs(other->GetMax().x - GetMin().x);
		float backDist = Math::Abs(other->GetMin().x - GetMax().x);
		float minDist = Min(topDist, bottomDist, leftDist, rightDist, frontDist, backDist);

		if (minDist == topDist)
		{
			offset.z += topDist;
			return CollSide::Top;
		}
		else if (minDist == bottomDist)
		{
			offset.z -= bottomDist;
			return CollSide::Bottom;
		}
		else if (minDist == rightDist)
		{
			offset.y += rightDist;
			return CollSide::Right;
		}
		else if (minDist == leftDist)
		{
			offset.y -= leftDist;
			return CollSide::Left;
		}
		else if (minDist == frontDist)
		{
			offset.x += frontDist;
			return CollSide::Front;
		}
		else if (minDist == backDist)
		{
			offset.x -= backDist;
			return CollSide::Back;
		}
	}

	return CollSide::None;
}
