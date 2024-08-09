//Header Include
#include "Player.h"

//Component Includes
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"
#include "CameraComponent.h"
#include "PlayerUI.h"

//Variable Includes
#include "Game.h"
#include "Renderer.h"

//Function Includes
#include "Mesh.h"

Player::Player(Game* game)
: Actor(game)
{
	SetScale(PLAYER_SCALE);

	MeshComponent* mesh = new MeshComponent(this);
	mesh->SetMesh(mGame->GetRenderer()->GetMesh(PLAYER_MESH));

	new PlayerMove(this);

	new CameraComponent(this);

	new PlayerUI(this);
}

Player::~Player()
{
}