//Header Include
#include "Player.h"

//Component Includes
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"

//Variable Includes
#include "Game.h"
#include "Renderer.h"
#include "HUD.h"

//Function Includes

Player::Player(Game* game)
: Actor(game)
{
	SetScale(PLAYER_SCALE);

	MeshComponent* mesh = new MeshComponent(this);
	mesh->SetMesh(mGame->GetRenderer()->GetMesh(PLAYER_MESH));

	CollisionComponent* collision = new CollisionComponent(this);
	collision->SetSize(PLAYER_COLLISION_SIZE.x, PLAYER_COLLISION_SIZE.y, PLAYER_COLLISION_SIZE.z);

	new PlayerMove(this);

	new HUD(this);
}

Player::~Player()
{
}