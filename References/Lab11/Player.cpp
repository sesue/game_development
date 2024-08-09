//Header Include
#include "Player.h"

//Component Includes
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"
#include "CameraComponent.h"
#include "HealthComponent.h"

//Variable Includes
#include "Game.h"
#include "Renderer.h"
#include "PlayerMesh.h"

//Function Includes
#include "Mesh.h"

Player::Player(Game* game, Vector3 initialPos, bool hasGun)
: Actor(game)
{
	if (hasGun)
		GiveGun();

	SetPosition(initialPos);
	mInitialPos = initialPos;

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(PLAYER_COLLISION_SIZE.x, PLAYER_COLLISION_SIZE.y, PLAYER_COLLISION_SIZE.z);

	new CameraComponent(this);

	new PlayerMove(this);

	HealthComponent* health = new HealthComponent(this);
	health->SetOnDeath([this] {
		mGame->ReloadLevel();
	});
}

Player::~Player()
{
}

void Player::GiveGun()
{
	mHasGun = true;

	new PlayerMesh(mGame);
}