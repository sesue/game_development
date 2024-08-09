//Header Include
#include "Block.h"

//Component Includes
#include "MeshComponent.h"
#include "CollisionComponent.h"

//Variable Includes
#include "Game.h"
#include "Renderer.h"
#include "Player.h"

//Function Includes

Block::Block(Game* game, bool isExploding)
: Actor(game)
, mIsExploding(isExploding)
{
	SetScale(BLOCK_SCALE);

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(COLLISION_BOX.x, COLLISION_BOX.y, COLLISION_BOX.z);

	MeshComponent* mesh = new MeshComponent(this);
	mesh->SetMesh(mGame->GetRenderer()->GetMesh(BLOCK_MESH));
	mesh->SetTextureIndex(!isExploding ? REGULAR_TEXTURE_INDEX : EXPLODING_TEXTURE_INDEX);

	game->AddBlock(this);
}

Block::~Block()
{
	mGame->RemoveBlock(this);
}

void Block::OnUpdate(float deltaTime)
{
	if (mGame->GetPlayer()->GetPosition().x > GetPosition().x + DESTROY_DISTANCE)
		SetState(ActorState::Destroy);
}

bool Block::Explode()
{
	if (!mIsExploding)
		return false;

	mIsExploding = false;

	Vector3 position = GetPosition();
	for (Block* block : mGame->GetBlocks())
	{
		if (Vector3::Distance(position, block->GetPosition()) <= EXPLOSION_DISTANCE)
		{
			block->Explode();
			block->SetState(ActorState::Destroy);
			SetState(ActorState::Destroy);
		}
	}

	return true;
}