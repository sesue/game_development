#pragma once
#include "UIComponent.h"
#include <string>
#include <vector>

class HUD : public UIComponent
{
public:
	HUD(class Actor* owner);
	~HUD();

	void Draw(class Shader* shader) override;
	void ShowSubtitle(std::string text);

	const std::string FONT_FILE = "Assets/Inconsolata-Regular.ttf";
	const Vector2 SHADOW_OFFSET = Vector2(2.0f, -2.0f);
	
	const std::string DAMAGE_INDICATOR_FILE = "Assets/Textures/UI/DamageIndicator.png";
	const float INDICATOR_TIME = 1.5f;
	const std::string DEATH_INDICATOR_FILE = "Assets/Textures/UI/DamageOverlay.png";

	void PlayerTakeDamage(float angle);
	void PlayerDie();

	void Update(float deltaTime) override;

private:
	class Font* mFont = nullptr;
	class Texture* mSubtitle = nullptr;
	class Texture* mShadow = nullptr;

	class Texture* mDamageIndicator;
	float mIndicatorAngle;
	float mIndicatorTime;

	class Texture* mDeathIndicator;
};