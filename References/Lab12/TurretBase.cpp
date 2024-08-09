//Baser Include
#include "TurretBase.h"

//Component Includes
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"

//Actor Includes
#include "Player.h"
#include "TurretHead.h"

//Variable Includes
#include "Game.h"
#include "Renderer.h"

//Function Includes
#include "Mesh.h"

TurretBase::TurretBase(Game* game)
: Actor(game)
{
	SetScale(INIT_SCALE);

	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh(MESH_FILE));

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(INIT_COLLISION.x, INIT_COLLISION.y, INIT_COLLISION.z);

	mHead = new TurretHead(game, this);

	HealthComponent* health = new HealthComponent(this);
	health->SetOnDeath([this] {
		Die();
	});
	health->SetOnDamage([this](const Vector3& location) {
		mHead->TakeDamage();
	});

	game->AddCollider(this);
}

TurretBase::~TurretBase()
{
	mGame->RemoveCollider(this);
}

void TurretBase::OnUpdate(float deltaTime)
{
}