//Header Include
#include "Bullet.h"

//Component Includes
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "MoveComponent.h"
#include "PlayerMove.h"

//Variable Includes
#include "Game.h"
#include "Renderer.h"
#include "Player.h"
#include "Block.h"

//Function Includes

Bullet::Bullet(Game* game, Vector3 position)
: Actor(game)
{
	mPosition = position;

	SetScale(BULLET_SCALE);

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(COLLISION_BOX.x, COLLISION_BOX.y, COLLISION_BOX.z);

	MeshComponent* mesh = new MeshComponent(this);
	mesh->SetMesh(mGame->GetRenderer()->GetMesh(BULLET_MESH));

	new MoveComponent(
		this, INITIAL_SPEED * game->GetPlayer()->GetComponent<PlayerMove>()->GetSpeedMultiplier());
}

Bullet::~Bullet()
{
}

void Bullet::OnUpdate(float deltaTime)
{
	if (mLifeTime < 0.0f)
		SetState(ActorState::Destroy);
	else
		mLifeTime -= deltaTime;

	CollisionComponent* bulletCollision = GetComponent<CollisionComponent>();
	for (Block* block : mGame->GetBlocks())
	{
		if (bulletCollision->Intersect(block->GetComponent<CollisionComponent>()))
		{
			if (block->Explode())
				mGame->GetAudio()->PlaySound("BlockExplode.wav");
			SetState(ActorState::Destroy);
			break;
		}
	}
}