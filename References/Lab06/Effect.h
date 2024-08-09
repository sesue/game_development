#pragma once

#include "Actor.h"
#include <string>

class Effect : public Actor
{
public:
	Effect(class Game* game, Vector2 position, std::string animationName, std::string soundName);
	~Effect();

	void OnUpdate(float deltaTime) override;

	const std::string EFFECT_ANIMATIONS = "Assets/Effects";

private:
	float mLifeTime = 0.0f;
};