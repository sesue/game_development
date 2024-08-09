#include "EnemyComponent.h"
#include "Game.h"
#include "Actor.h"
#include "CollisionComponent.h"
#include <algorithm>

EnemyComponent::EnemyComponent(class Actor* owner, int health)
: Component(owner)
, mHitPoints(health)
{
	owner->GetGame()->AddEnemyComponent(this);
	mCollision = owner->GetComponent<CollisionComponent>();
}

EnemyComponent::~EnemyComponent()
{
	mOwner->GetGame()->RemoveEnemyComponent(this);
}

void EnemyComponent::Update(float deltaTime)
{
	mPrevDamageTimer += deltaTime;
}

void EnemyComponent::TakeDamage()
{
	if (mPrevDamageTimer > INVULNERABILITY_TIME)
	{
		mHitPoints -= 1;
		if (mOnDamage)
			mOnDamage();
	}

	if (mHitPoints == 0)
	{
		if (mOnDeath)
			mOnDeath();
		Die();
	}

	mPrevDamageTimer = 0.0f;
}

void EnemyComponent::Die()
{
	mOwner->SetState(ActorState::Destroy);
}