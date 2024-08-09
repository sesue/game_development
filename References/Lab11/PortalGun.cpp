//Header Include
#include "PortalGun.h"

//Actor Include
#include "Player.h"

//Component Includes
#include "MeshComponent.h"
#include "CollisionComponent.h"

//Variable Includes
#include "Game.h"
#include "Renderer.h"

//Function Includes
#include "Mesh.h"

PortalGun::PortalGun(Game* game)
: Actor(game)
{
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh(GUN_MESH));

	mCollision = new CollisionComponent(this);
	mCollision->SetSize(GUN_COLLISION_SIZE.x, GUN_COLLISION_SIZE.y, GUN_COLLISION_SIZE.z);
}

PortalGun::~PortalGun()
{
}

void PortalGun::OnUpdate(float deltaTime)
{
	mRotation += Math::Pi * deltaTime;

	if (mCollision->Intersect(mGame->GetPlayer()->GetComponent<CollisionComponent>()))
	{
		mGame->GetPlayer()->GiveGun();
		SetState(ActorState::Destroy);
	}
}