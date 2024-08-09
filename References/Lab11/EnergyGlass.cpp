//Header Include
#include "EnergyGlass.h"

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

EnergyGlass::EnergyGlass(Game* game)
: Actor(game)
{
	MeshComponent* mc = new MeshComponent(this, ALPHA_MESH);
	mc->SetMesh(mGame->GetRenderer()->GetMesh(MESH_FILE));
	mc->SetTextureIndex(TEXTURE_INDEX);

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(COLLISION_BOX.x, COLLISION_BOX.y, COLLISION_BOX.z);

	game->AddCollider(this);
}

EnergyGlass::~EnergyGlass()
{
	mGame->RemoveCollider(this);
}

void EnergyGlass::OnUpdate(float deltaTime)
{
}