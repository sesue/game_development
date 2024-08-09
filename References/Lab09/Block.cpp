//Header Include
#include "Block.h"

//Component Includes
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "CameraComponent.h"

//Variable Includes
#include "Game.h"
#include "Renderer.h"

//Function Includes
#include "Mesh.h"

Block::Block(Game* game)
: Actor(game)
{
	SetScale(BLOCK_SCALE);

	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh(BLOCK_MESH));

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(BLOCK_COLLISION_SIZE.x, BLOCK_COLLISION_SIZE.y, BLOCK_COLLISION_SIZE.z);

	game->AddCollider(this);
}

Block::~Block()
{
	mGame->RemoveCollider(this);
}