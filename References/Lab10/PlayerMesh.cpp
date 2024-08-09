//Header Include
#include "PlayerMesh.h"

//Component Includes
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "CameraComponent.h"

//Variable Includes
#include "Game.h"
#include "Renderer.h"
#include "Player.h"

//Function Includes
#include "Mesh.h"
#include "MeshComponent.h"

PlayerMesh::PlayerMesh(Game* game)
: Actor(game)
{
	SetScale(PLAYER_MESH_SCALE);

	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh(PLAYER_MESH));
}

PlayerMesh::~PlayerMesh()
{
}

void PlayerMesh::OnUpdate(float deltaTime)
{
	Vector3 unproject = mGame->GetRenderer()->Unproject(Vector3(300.0f, -250.0f, 0.4f));
	SetPosition(unproject);

	Player* player = mGame->GetPlayer();
	Quaternion pitchQuat = Quaternion(Vector3::UnitY,
									  player->GetComponent<CameraComponent>()->GetPitchAngle());
	Quaternion yawQuat = Quaternion(Vector3::UnitZ, player->GetRotation());
	SetQuat(Quaternion::Concatenate(pitchQuat, yawQuat));
}