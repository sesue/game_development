#include "HealthComponent.h"

#include "Actor.h"
#include "Player.h"

#include "SDL2/SDL.h"

HealthComponent::HealthComponent(class Actor* owner, float health)
: Component(owner, 50)
, mHealth(health)
{
}

void HealthComponent::Update(float deltaTime)
{
}

void HealthComponent::TakeDamage(float damage, const Vector3& location)
{
	if (mHealth <= 0.0f)
		return;

	mHealth -= damage;
	
	if (mOnDamage != nullptr)
		mOnDamage(location);

	if (mHealth <= 0.0f && mOnDeath != nullptr)
		mOnDeath();
}