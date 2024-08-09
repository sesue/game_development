#pragma once
#include "Component.h"
#include <functional>

class HealthComponent : public Component
{
public:
	HealthComponent(class Actor* owner, float maxHealth = 100.0f);

	// Update the move component
	void Update(float deltaTime) override;

	//Setters & Getters
	void SetOnDamage(std::function<void(const Vector3&)> func) { mOnDamage = func; }
	void SetOnDeath(std::function<void()> func) { mOnDeath = func; }
	const float& GetHealth() const { return mHealth; }

	bool IsDead() const { return mHealth <= 0.0f; }
	void TakeDamage(float damage, const Vector3& location);

private:
	float mHealth = 0.0f;

	std::function<void(const Vector3&)> mOnDamage = nullptr;
	std::function<void()> mOnDeath = nullptr;
};
