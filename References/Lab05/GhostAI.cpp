#include "GhostAI.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <SDL2/SDL.h>
#include <unordered_map>
#include "Ghost.h"
#include "PacMan.h"
#include "Random.h"

GhostAI::GhostAI(class Actor* owner)
: Component(owner, 50)
{
	mGhost = static_cast<Ghost*>(owner);
}

//Helper Functions
Vector2 GhostAI::CalcDirection()
{
	Vector2 direction = Vector2::Zero;
	if (mNextNode != nullptr)
		direction = Vector2::Normalize(mNextNode->GetPosition() - mPrevNode->GetPosition());

	if (mState != Frightened)
	{
		std::string prefix;
		if (mState == Dead)
			prefix = "dead";

		bool isHorizontal = Math::Abs(direction.x) > Math::Abs(direction.y);
		bool isNegative = (isHorizontal * direction.x + !isHorizontal * direction.y) < 0.0f;

		AnimatedSprite* ghostSprite = mGhost->GetComponent<AnimatedSprite>();

		if (isHorizontal && isNegative)
			ghostSprite->SetAnimation(prefix + "left");
		else if (isHorizontal && !isNegative)
			ghostSprite->SetAnimation(prefix + "right");
		else if (!isHorizontal && isNegative)
			ghostSprite->SetAnimation(prefix + "up");
		else
			ghostSprite->SetAnimation(prefix + "down");
	}

	return direction;
}

//State Change Functions
void GhostAI::StateChange()
{
	//Switch Frightened Animation and from Frightened to Scatter
	if (mState == Frightened && mStateTime > FRIGHTENED_0_TIME + FRIGHTENED_1_TIME)
	{
		mState = Scatter;
		mCurrentSpeed = NORMAL_SPEED;
		mStateTime = 0.0f;
	}
	else if (mState == Frightened && mStateTime > FRIGHTENED_0_TIME)
	{
		mGhost->GetComponent<AnimatedSprite>()->SetAnimation("scared1");
	}

	//Switch from Dead to Scatter
	if (mState == Dead && mNextNode == GetGame()->GetGhostPen())
	{
		mState = Scatter;
		mCurrentSpeed = NORMAL_SPEED;
		mStateTime = 0.0f;
	}

	//Switch from Scatter to Chase
	if (mState == Scatter && mStateTime > SCATTER_TIME)
	{
		mState = Chase;
		mStateTime = 0.0f;
	}

	//Switch from Chase to Scatter
	if (mState == Chase && mStateTime > CHASE_TIME)
	{
		mState = Scatter;
		mStateTime = 0.0f;
	}
}

void GhostAI::Frighten()
{
	//Do nothing it Dead
	if (mState == Dead)
		return;

	mStateTime = 0.0f;

	if (mState != Frightened)
	{
		mState = Frightened;
		mCurrentSpeed = FRIGHT_SPEED;

		PathNode* tempNode = mNextNode;
		mNextNode = mPrevNode;
		mPrevNode = tempNode;
		mDirection = CalcDirection();

		mTargetNode = nullptr;

		mGhost->GetComponent<AnimatedSprite>()->SetAnimation("scared0");
	}
}

void GhostAI::Start(PathNode* startNode)
{
	mGhost->SetPosition(startNode->GetPosition());
	mState = Scatter;
	mPrevNode = nullptr;
	mNextNode = startNode;
	mStateTime = 0.0f;
}

void GhostAI::Die()
{
	mStateTime = 0.0f;

	if (mState != Dead)
	{
		mState = Dead;
		mCurrentSpeed = DEAD_SPEED;

		mTargetNode = GetGame()->GetGhostPen();

		mDirection = CalcDirection();

		mGhost->GetComponent<AnimatedSprite>()->SetAnimation("scared0");
	}
}

//Movement Functions
void GhostAI::UpdateMovementNodes()
{
	PathNode* newNextNode = nullptr;
	float minDistance = Math::Infinity;

	//Check Adjacent Node that are not the Previous node or a Ghost or Tunnel Node
	for (PathNode* node : mNextNode->mAdjacent)
	{
		if (node != mPrevNode && ((node->GetType() == PathNode::Default) ||
								  (mState == Dead && node->GetType() != PathNode::Tunnel)))
		{
			if (Vector2::Distance(node->GetPosition(), mTargetNode->GetPosition()) < minDistance)
			{
				minDistance = Vector2::Distance(node->GetPosition(), mTargetNode->GetPosition());
				newNextNode = node;
			}
		}
	}

	//None found so Check Adjacent Node that are not the Previous node or a Tunnel Node
	if (newNextNode == nullptr)
	{
		for (PathNode* node : mNextNode->mAdjacent)
		{
			if (node != mPrevNode && node->GetType() != PathNode::Tunnel)
			{
				if (Vector2::Distance(node->GetPosition(), mTargetNode->GetPosition()) <
					minDistance)
				{
					minDistance = Vector2::Distance(node->GetPosition(),
													mTargetNode->GetPosition());
					newNextNode = node;
				}
			}
		}
	}

	//None found so take any adjacent node
	if (newNextNode == nullptr)
	{
		for (PathNode* node : mNextNode->mAdjacent)
		{
			if (Vector2::Distance(node->GetPosition(), mTargetNode->GetPosition()) < minDistance)
			{
				minDistance = Vector2::Distance(node->GetPosition(), mTargetNode->GetPosition());
				newNextNode = node;
			}
		}
	}

	mPrevNode = mNextNode;
	mNextNode = newNextNode;
}

//Ghost Target Functions
void GhostAI::FrightenedTargetNode()
{
	//Choose random adjacent node that is not the Previous node or a Ghost or Tunnel Node
	int start = Random::GetIntRange(0, static_cast<int>(mNextNode->mAdjacent.size()));
	for (int i = start; i < start + mNextNode->mAdjacent.size(); i++)
	{
		if (mNextNode->mAdjacent[i % mNextNode->mAdjacent.size()] != mPrevNode &&
			mNextNode->mAdjacent[i % mNextNode->mAdjacent.size()]->GetType() == PathNode::Default)
			mTargetNode = mNextNode->mAdjacent[i % mNextNode->mAdjacent.size()];
	}

	if (mTargetNode == nullptr)
	{
		for (int i = start; i < start + mNextNode->mAdjacent.size(); i++)
		{
			if (mNextNode->mAdjacent[i % mNextNode->mAdjacent.size()] != mPrevNode &&
				mNextNode->mAdjacent[i % mNextNode->mAdjacent.size()]->GetType() !=
					PathNode::Tunnel)
				mTargetNode = mNextNode->mAdjacent[i % mNextNode->mAdjacent.size()];
		}
	}

	if (mTargetNode == nullptr)
		mTargetNode = mNextNode->mAdjacent[start];
}

void GhostAI::TargetClosestDefault(Vector2 targetPosition)
{
	float minDistance = Math::Infinity;
	PathNode* closestNode = nullptr;
	for (PathNode* node : GetGame()->GetPathNodes())
	{
		if (node->GetType() == PathNode::Default &&
			Vector2::Distance(node->GetPosition(), targetPosition) < minDistance)
		{
			minDistance = Vector2::Distance(node->GetPosition(), mTargetNode->GetPosition());
			mTargetNode = node;
		}
	}
}

void GhostAI::BlinkyTargetNode()
{
	mTargetNode = GetGame()->GetPlayer()->GetPrevNode();

	if (mTargetNode->GetType() == PathNode::Tunnel)
	{
		TargetClosestDefault(GetGame()->GetPlayer()->GetPosition());
	}
}

void GhostAI::PinkyTargetNode()
{
	TargetClosestDefault(GetGame()->GetPlayer()->GetPointInFrontOf(PINKY_TARGET_DISTANCE));
}

void GhostAI::InkyTargetNode()
{
	//Get vector from Blinky's Position to Point in front of Pac-Man
	//Double it and Add it to Blinky's Position for target Point
	Vector2 v = GetGame()->GetPlayer()->GetPointInFrontOf(INKY_TARGET_DISTANCE) -
				GetGame()->GetGhosts()[0]->GetPosition();
	v = v * 2;
	TargetClosestDefault(GetGame()->GetGhosts()[0]->GetPosition() + v);
}

void GhostAI::ClydeTargetNode()
{
	float dist = Vector2::Distance(mGhost->GetPosition(), GetGame()->GetPlayer()->GetPosition());

	if (dist > CLYDE_TARGET_THRESHOLD)
		mTargetNode = GetGame()->GetPlayer()->GetPrevNode();
	else
		mTargetNode = mGhost->GetScatterNode();
}

void GhostAI::UpdateTargetNode()
{
	if (mState == Scatter)
		mTargetNode = mGhost->GetScatterNode();
	else if (mState == Frightened)
		FrightenedTargetNode();
	else if (mState == Chase)
	{
		if (mGhost->GetType() == Ghost::Blinky)
			BlinkyTargetNode();
		else if (mGhost->GetType() == Ghost::Pinky)
			PinkyTargetNode();
		else if (mGhost->GetType() == Ghost::Inky)
			InkyTargetNode();
		else
			ClydeTargetNode();
	}
}

//General Update Function
void GhostAI::Update(float deltaTime)
{
	//Update State Time
	mStateTime += deltaTime;

	//Update Position
	mGhost->SetPosition(mGhost->GetPosition() + (mDirection * mCurrentSpeed * deltaTime));

	//Check for Intersection with next Node
	if (mGhost->GetComponent<CollisionComponent>()->Intersect(
			mNextNode->GetComponent<CollisionComponent>()))
	{
		mGhost->SetPosition(mNextNode->GetPosition());

		//Check for State Change
		StateChange();

		//Update Target Node
		UpdateTargetNode();

		//Change mPrevNode and mNextNode
		UpdateMovementNodes();
		mDirection = CalcDirection();
	}
}

//Debug Function
void GhostAI::DebugDrawPath(SDL_Renderer* render)
{
	// Draw a rectangle at the target node
	if (mTargetNode != nullptr)
	{
		const int SIZE = 16;
		SDL_Rect r;
		r.x = static_cast<int>(mTargetNode->GetPosition().x) - SIZE / 2;
		r.y = static_cast<int>(mTargetNode->GetPosition().y) - SIZE / 2;
		r.w = SIZE;
		r.h = SIZE;
		SDL_RenderDrawRect(render, &r);
	}

	// Line from ghost to next node
	if (mNextNode != nullptr)
	{
		SDL_RenderDrawLine(render, static_cast<int>(mOwner->GetPosition().x),
						   static_cast<int>(mOwner->GetPosition().y),
						   static_cast<int>(mNextNode->GetPosition().x),
						   static_cast<int>(mNextNode->GetPosition().y));
	}
}
