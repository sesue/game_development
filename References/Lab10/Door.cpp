//Header Include
#include "Door.h"

//Component Includes
#include "MeshComponent.h"
#include "CollisionComponent.h"

//Variable Includes
#include "Game.h"
#include "Renderer.h"

//Function Includes
#include "Mesh.h"

Door::Door(Game* game, std::string name)
: Actor(game)
, mName(name)
{
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = mGame->GetRenderer()->GetMesh(DOOR_MESH);
	mc->SetMesh(mesh);

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(mesh->GetWidth(), mesh->GetHeight(), mesh->GetDepth());

	game->AddCollider(this);
	game->AddDoor(this);
}

Door::~Door()
{
	if (mIsOpen)
		mGame->RemoveCollider(this);
}

void Door::Open()
{
	mIsOpen = true;
	mGame->RemoveCollider(this);
}