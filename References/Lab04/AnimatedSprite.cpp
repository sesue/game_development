#include "AnimatedSprite.h"
#include "Actor.h"
#include "Game.h"
#include <string>

AnimatedSprite::AnimatedSprite(Actor* owner, int drawOrder)
: SpriteComponent(owner, drawOrder)
{
}

void AnimatedSprite::Update(float deltaTime)
{
	if (mAnimName.empty())
		return;

	if (!mIsPaused)
	{
		mAnimTimer += mAnimFPS * deltaTime;
	}

	if (mAnimTimer >= mAnims[mAnimName].size())
		mAnimTimer = 0.0f;

	SetTexture(mAnims[mAnimName][static_cast<int>(mAnimTimer)]);
}

void AnimatedSprite::AddAnimation(const std::string& name, const std::vector<SDL_Texture*>& images)
{
	mAnims.emplace(name, images);
}
