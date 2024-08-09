//Header Include
#include "SideBlock.h"

//Component Includes
#include "MeshComponent.h"

//Variable Includes
#include "Game.h"
#include "Renderer.h"
#include "Player.h"

//Function Includes

SideBlock::SideBlock(Game* game, size_t textureIndex)
: Actor(game)
{
	SetScale(SIDEBLOCK_SCALE);

	MeshComponent* mesh = new MeshComponent(this);
	mesh->SetMesh(mGame->GetRenderer()->GetMesh(SIDEBLOCK_MESH));
	mesh->SetTextureIndex(textureIndex);
}

SideBlock::~SideBlock()
{
}

void SideBlock::OnUpdate(float deltaTime)
{
	if (mGame->GetPlayer()->GetPosition().x > GetPosition().x + DESTROY_DISTANCE)
		SetState(ActorState::Destroy);
}