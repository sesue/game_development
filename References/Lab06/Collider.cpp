#include "Collider.h"
#include "CollisionComponent.h"
#include "Game.h"

Collider::Collider(Game* game, float width, float height)
: Actor(game)
{
	game->AddCollider(this);

	CollisionComponent* collision = new CollisionComponent(this);
	collision->SetSize(width, height);
}

Collider::~Collider()
{
	GetGame()->RemoveCollider(this);
}