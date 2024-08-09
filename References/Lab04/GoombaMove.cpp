#include "GoombaMove.h"
#include "Actor.h"
#include "Game.h"
#include "Goomba.h"
#include "Block.h"
#include "CollisionComponent.h"

GoombaMove::GoombaMove(Goomba* owner, CollisionComponent* collision)
: MoveComponent(owner, 100.0f)
, mVelocity(Vector2(-100.0f, 0.0f))
, mOwnerCollision(collision)
{
}

GoombaMove::~GoombaMove()
{
}

void GoombaMove::Update(float deltaTime)
{
	if (!(static_cast<Goomba*>(mOwner)->GetStomped()))
	{
		//Set Position Based on X-Y-Velocity
		mOwner->SetPosition(Vector2(mOwner->GetPosition().x + (mVelocity.x * deltaTime),
									mOwner->GetPosition().y + (mVelocity.y * deltaTime)));

		//Apply Gravity
		mVelocity.y += GetGame()->GAME_GRAVITY * deltaTime;

		//Check for Collisions
		for (Block* block : GetGame()->GetBlocks())
		{
			Vector2 offset = Vector2::Zero;
			CollSide side = mOwnerCollision->GetMinOverlap(block->GetCollision(), offset);
			if (side == CollSide::Top && mVelocity.y > 0.0f)
				mVelocity.y = 0;

			mOwner->SetPosition(mOwner->GetPosition() + offset);
		}

		for (Block* block : GetGame()->GetBlocks())
		{
			Vector2 offset = Vector2::Zero;
			CollSide side = mOwnerCollision->GetMinOverlap(block->GetCollision(), offset);
			if ((side == CollSide::Right || side == CollSide::Left))
				mVelocity.x = -mVelocity.x;

			mOwner->SetPosition(mOwner->GetPosition() + offset);
		}

		for (Goomba* goomba : GetGame()->GetEnemies())
		{
			if (goomba != mOwner)
			{
				Vector2 offset = Vector2::Zero;
				CollSide side = mOwnerCollision->GetMinOverlap(goomba->GetCollision(), offset);
				if (side != CollSide::None)
				{
					if (side == CollSide::Top && mVelocity.y > 0.0f)
						mVelocity.y = 0;
					if (side == CollSide::Right || side == CollSide::Left)
						mVelocity.x = -mVelocity.x;

					mOwner->SetPosition(mOwner->GetPosition() + offset);
				}
			}
		}

		//Prevent falling through bottom of window
		if (mOwner->GetPosition().y > GetGame()->GAME_FLOOR)
			mOwner->SetState(ActorState::Destroy);
	}
}