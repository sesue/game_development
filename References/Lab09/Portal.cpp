#include "Portal.h"
#include "Game.h"
#include "PortalMeshComponent.h"
#include "Renderer.h"

Portal::Portal(class Game* game, bool isBlue)
: Actor(game)
{
	PortalMeshComponent* portalMesh = new PortalMeshComponent(this);
	portalMesh->SetTextureIndex((isBlue ? 0 : 1));
	MeshComponent* mesh = new MeshComponent(this, true);
	mesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Meshes/Portal.gpmesh"));
	mesh->SetTextureIndex(isBlue ? 2 : 3);
}
