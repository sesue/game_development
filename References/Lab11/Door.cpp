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

	mLeftHalf = new Actor(game, this);
	mRightHalf = new Actor(game, this);

	MeshComponent* leftMC = new MeshComponent(mLeftHalf);
	leftMC->SetMesh(mGame->GetRenderer()->GetMesh(DOOR_LEFT_MESH));

	MeshComponent* rightMC = new MeshComponent(mRightHalf);
	rightMC->SetMesh(mGame->GetRenderer()->GetMesh(DOOR_RIGHT_MESH));
}

Door::~Door()
{
	if (mIsOpen)
		mGame->RemoveCollider(this);
}

void Door::OnUpdate(float deltaTime)
{
	if (mIsOpen && mOpenTime < 1.0f)
	{
		mOpenTime = Math::Clamp(mOpenTime + deltaTime, 0.0f, 1.0f);
		Vector3 position = Vector3::Lerp(Vector3::Zero, FULL_OPEN, mOpenTime);
		mRightHalf->SetPosition(position);
		mLeftHalf->SetPosition(-1 * position);
	}
}

void Door::Open()
{
	mIsOpen = true;
	mGame->RemoveCollider(this);
}