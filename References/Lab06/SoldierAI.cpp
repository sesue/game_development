#include "SoldierAI.h"
#include "Actor.h"
#include "Game.h"
#include "PathFinder.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <algorithm>

SoldierAI::SoldierAI(Actor* owner)
: SpriteComponent(owner)
{
	SetIsVisible(false); // Uncomment to hide debug paths
}

void SoldierAI::Setup(PathNode* start, PathNode* end)
{
	mPatrolStart = start;
	mPatrolEnd = end;

	GetOwner()->GetGame()->GetPaths()->CalculatePath(mPatrolStart, mPatrolEnd, mPath);

	mPrev = mPatrolStart;
	mNext = mPath.back();
	mPath.pop_back();
}

void SoldierAI::AnimationUpdate(float deltaTime)
{
	AnimatedSprite* soldierSprite = mOwner->GetComponent<AnimatedSprite>();

	if (mStunTimer <= 0.0f)
	{
		soldierSprite->SetIsPaused(false);

		bool isHorizontal = Math::Abs(mDirection.x) > Math::Abs(mDirection.y);
		bool isNegative = (isHorizontal * mDirection.x + !isHorizontal * mDirection.y) < 0.0f;

		if (isHorizontal && isNegative)
			soldierSprite->SetAnimation("WalkLeft");
		else if (isHorizontal && !isNegative)
			soldierSprite->SetAnimation("WalkRight");
		else if (!isHorizontal && isNegative)
			soldierSprite->SetAnimation("WalkUp");
		else
			soldierSprite->SetAnimation("WalkDown");
	}
	else
	{
		soldierSprite->SetIsPaused(true);
	}
}

void SoldierAI::Stun()
{
	mStunTimer = STUN_DURATION;
}

void SoldierAI::Update(float deltaTime)
{
	if (mStunTimer <= 0.0f)
	{
		Vector2 position = mOwner->GetPosition();
		Vector2 nextPosition = mNext->GetPosition();

		//Check if Soldier has Arrived at Next Node
		if (Vector2::Distance(position, nextPosition) <= ARRIVAL_DISTANCE)
		{
			mOwner->SetPosition(nextPosition);

			if (mPath.empty())
			{
				Setup(mPatrolEnd, mPatrolStart);
			}
			mPrev = mNext;
			mNext = mPath.back();
			mPath.pop_back();
			nextPosition = mNext->GetPosition();
		}

		//Calculate Normalized Direction to Move Soldier
		mDirection = Vector2::Normalize(nextPosition - position);

		//Move Soldier Position
		mOwner->SetPosition(position + (mDirection * SOLDIER_SPEED * deltaTime));
	}
	else
		mStunTimer -= deltaTime;

	AnimationUpdate(deltaTime);
}

// This helper is to just debug draw the soldier's path to visualize it
// (only called if this component is set to visible)
void SoldierAI::Draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	Vector2 cameraPos = mOwner->GetGame()->GetCameraPos();

	// Draw from current position to next
	SDL_RenderDrawLine(renderer, static_cast<int>(mOwner->GetPosition().x - cameraPos.x),
					   static_cast<int>(mOwner->GetPosition().y - cameraPos.y),
					   static_cast<int>(mNext->GetPosition().x - cameraPos.x),
					   static_cast<int>(mNext->GetPosition().y - cameraPos.y));

	// Draw from next to first node on path
	if (!mPath.empty())
	{
		// Draw from current position to next
		SDL_RenderDrawLine(renderer, static_cast<int>(mNext->GetPosition().x - cameraPos.x),
						   static_cast<int>(mNext->GetPosition().y - cameraPos.y),
						   static_cast<int>(mPath.back()->GetPosition().x - cameraPos.x),
						   static_cast<int>(mPath.back()->GetPosition().y - cameraPos.y));
	}

	// Draw each node on the path
	if (mPath.size() > 1)
	{
		for (size_t i = 0; i < mPath.size() - 1; i++)
		{
			SDL_RenderDrawLine(renderer, static_cast<int>(mPath[i]->GetPosition().x - cameraPos.x),
							   static_cast<int>(mPath[i]->GetPosition().y - cameraPos.y),
							   static_cast<int>(mPath[i + 1]->GetPosition().x - cameraPos.x),
							   static_cast<int>(mPath[i + 1]->GetPosition().y - cameraPos.y));
		}
	}
}
