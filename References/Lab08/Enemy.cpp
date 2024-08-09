//Header Include
#include "Enemy.h"

//Component Includes
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "EnemyMove.h"

//Variable Includes
#include "Game.h"
#include "Renderer.h"

//Function Includes
#include "Mesh.h"

Enemy::Enemy(Game* game)
: Actor(game)
{
	SetScale(ENEMY_SCALE);

	MeshComponent* mesh = new MeshComponent(this);
	mesh->SetMesh(mGame->GetRenderer()->GetMesh(ENEMY_MESH));
	mesh->SetTextureIndex(ENEMY_TEXTURE_INDEX);

	new EnemyMove(this);
}

Enemy::~Enemy()
{
}