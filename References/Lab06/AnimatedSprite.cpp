#include "AnimatedSprite.h"
#include "Actor.h"
#include "Game.h"
#include <string>
#include <filesystem>
#include <algorithm>

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

void AnimatedSprite::LoadAnimations(const std::string& rootPath)
{
#ifndef __clang_analyzer__
	Game* game = mOwner->GetGame();
	for (const auto& rootDirEntry : std::filesystem::directory_iterator{rootPath})
	{
		// If this is a directory, it's a multi-frame animation so need to load all the frames
		if (rootDirEntry.is_directory())
		{
			// Load the file names into the vector
			std::vector<std::string> fileNames;
			for (const auto& animDirEntry : std::filesystem::directory_iterator{rootDirEntry})
			{
				if (animDirEntry.path().extension().string() == ".png")
				{
					fileNames.emplace_back(animDirEntry.path().string());
				}
			}

			// Technically the order is undefined, so sort just in case
			std::sort(fileNames.begin(), fileNames.end());

			// Now load all the textures
			std::vector<SDL_Texture*> images;
			for (const auto& file : fileNames)
			{
				images.emplace_back(game->GetTexture(file));
			}

			// Now add the animation using the directory name as the animation name
			AddAnimation(rootDirEntry.path().filename().string(), images);
		}
		// Non-directory means single-frame animation
		else if (rootDirEntry.path().extension().string() == ".png")
		{
			std::vector<SDL_Texture*> images;
			images.emplace_back(game->GetTexture(rootDirEntry.path().string()));
			AddAnimation(rootDirEntry.path().stem().string(), images);
		}
	}
#endif
}