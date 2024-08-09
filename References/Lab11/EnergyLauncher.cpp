//Header Include
#include "EnergyLauncher.h"

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

EnergyLauncher::EnergyLauncher(Game* game, float cooldown = 1.5f, std::string door = "")
: Actor(game)
, mCooldown(cooldown)
, mDoor(door)
{
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh(ENERGY_LAUNCHER_MESH));

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(COLLISION_SIZE, COLLISION_SIZE, COLLISION_SIZE);

	game->AddCollider(this);
}

EnergyLauncher::~EnergyLauncher()
{
	mGame->RemoveCollider(this);
}

void EnergyLauncher::OnUpdate(float deltaTime)
{
	if (mIsActive)
	{
		if (mTimer > mCooldown)
		{
			Pellet* pellet = new Pellet(mGame, GetForward());
			pellet->SetPosition(mPosition);
			pellet->CalcWorldTransform();
			mTimer = 0.0f;
		}
		else
			mTimer += deltaTime;

		Door* door = mGame->GetDoor(mDoor);
		if (door != nullptr && door->IsOpen())
			mIsActive = false;
	}
}