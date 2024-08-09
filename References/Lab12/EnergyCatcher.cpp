//Header Include
#include "EnergyCatcher.h"

//Component Includes
#include "MeshComponent.h"
#include "CollisionComponent.h"

//Actor Includes
#include "Door.h"

//Variable Includes
#include "Game.h"
#include "Renderer.h"

//Function Includes
#include "Mesh.h"

EnergyCatcher::EnergyCatcher(Game* game, std::string door = "")
: Actor(game)
, mDoor(door)
{
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh(ENERGYCATCHER_MESH));

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(COLLISION_SIZE, COLLISION_SIZE, COLLISION_SIZE);

	if (door == "")
		mActivated = true;

	game->AddCollider(this);
}

EnergyCatcher::~EnergyCatcher()
{
	mGame->RemoveCollider(this);
}

void EnergyCatcher::Activate()
{
	mGame->GetDoor(mDoor)->Open();
	mGame->GetAudio()->PlaySound(ACTIVATE_SOUND, false, this);
}