#pragma once
#include "Component.h"
#include "Math.h"
#include <functional>

class EnemyComponent : public Component
{
public:
	EnemyComponent(class Actor* owner, int health);
	~EnemyComponent();

	void Update(float deltaTimer) override;

	void SetOnDeath(std::function<void()> onDeath) { mOnDeath = onDeath; }
	void SetOnDamage(std::function<void()> onDamage) { mOnDamage = onDamage; }
	class CollisionComponent* GetCollision() { return mCollision; }

	void TakeDamage();

	const float INVULNERABILITY_TIME = 0.25f;

private:
	void SetHitPionts(int hitPoints) { mHitPoints = hitPoints; }
	const int GetHitPoints() const { return mHitPoints; }

	void Die();

	std::function<void()> mOnDamage;
	std::function<void()> mOnDeath;

	class CollisionComponent* mCollision = nullptr;
	int mHitPoints = 0;
	float mPrevDamageTimer = 0.0f;
};
