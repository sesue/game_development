//Header Include
#include "EnergyCube.h"

//Component Includes
#include "MeshComponent.h"
#include "CollisionComponent.h"

//Actor Includes
#include "Pellet.h"
#include "Door.h"

//Variable Includes
#include "Game.h"
#include "Renderer.h"

//Function Includes
#include "Mesh.h"

EnergyCube::EnergyCube(Game* game)
: Actor(game)
{
	MeshComponent* mc = new MeshComponent(this, ALPHA_MESH);
	mc->SetMesh(mGame->GetRenderer()->GetMesh(MESH_FILE));

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(COLLISION_BOX.x, COLLISION_BOX.y, COLLISION_BOX.z);

	game->AddCollider(this);
}

EnergyCube::~EnergyCube()
{
	mGame->RemoveCollider(this);
}

void EnergyCube::OnUpdate(float deltaTime)
{
}