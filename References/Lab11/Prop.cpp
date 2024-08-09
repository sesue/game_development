//Header Include
#include "Prop.h"

//Component Includes
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "CameraComponent.h"

//Variable Includes
#include "Game.h"
#include "Renderer.h"

//Function Includes
#include "Mesh.h"

Prop::Prop(class Game* game, bool alpha, bool collision, std::string meshStr)
: Actor(game)
{
	MeshComponent* mc = new MeshComponent(this, alpha);
	Mesh* meshPtr = mGame->GetRenderer()->GetMesh(meshStr);
	mc->SetMesh(meshPtr);

	if (collision)
	{
		CollisionComponent* cc = new CollisionComponent(this);
		cc->SetSize(meshPtr->GetWidth(), meshPtr->GetHeight(), meshPtr->GetDepth());

		game->AddCollider(this);
	}
}

Prop::~Prop()
{
	if (GetComponent<CollisionComponent>() != nullptr)
		mGame->RemoveCollider(this);
}