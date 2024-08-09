// Header Include
#include "HUD.h"

// Actor Includes
#include "Actor.h"
#include "Player.h"

// Component Includes
#include "HealthComponent.h"

// Utility Includes
#include "Shader.h"
#include "Font.h"
#include "Texture.h"
#include "Game.h"
#include "Renderer.h"

HUD::HUD(Actor* owner)
: UIComponent(owner)
{
	mFont = new Font();
	mFont->Load(FONT_FILE);

	mDamageIndicator = owner->GetGame()->GetRenderer()->GetTexture(DAMAGE_INDICATOR_FILE);
	mDeathIndicator = owner->GetGame()->GetRenderer()->GetTexture(DEATH_INDICATOR_FILE);
}

HUD::~HUD()
{
	mFont->Unload();
	delete mFont;
}

void HUD::Draw(Shader* shader)
{
	if (mSubtitle != nullptr)
	{
		const Vector2 position = Vector2(0.0f, -325.0f + mSubtitle->GetHeight() / 2.0f);
		DrawTexture(shader, mShadow, position + SHADOW_OFFSET);
		DrawTexture(shader, mSubtitle, position);
	}

	if (mIndicatorTime > 0.0f)
		DrawTexture(shader, mDamageIndicator, Vector2::Zero, 1.0f, mIndicatorAngle);

	if (mOwner->GetGame()->GetPlayer()->GetComponent<HealthComponent>()->IsDead())
		DrawTexture(shader, mDeathIndicator);
}

void HUD::ShowSubtitle(std::string text)
{
	if (mSubtitle != nullptr)
	{
		mSubtitle->Unload();
		mShadow->Unload();

		delete mSubtitle;
		delete mShadow;

		mSubtitle = nullptr;
		mShadow = nullptr;
	}

	if (!text.empty())
	{
		const std::string subtitle = "GLaDOS: " + text;
		mSubtitle = mFont->RenderText(subtitle, Color::LightGreen);
		mShadow = mFont->RenderText(subtitle, Color::Black);
	}
}

void HUD::PlayerTakeDamage(float angle)
{
	mIndicatorAngle = angle;
	mIndicatorTime = INDICATOR_TIME;
}

void HUD::PlayerDie()
{

}

void HUD::Update(float deltaTime)
{
	mIndicatorTime -= deltaTime;
}