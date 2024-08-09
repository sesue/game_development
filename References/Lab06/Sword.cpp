#include "Sword.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "AudioSystem.h"

Sword::Sword(Game* game)
: Actor(game)
{
	CollisionComponent* collision = new CollisionComponent(this);
	collision->SetSize(SWORD_WIDTH, SWORD_HEIGHT);
}

Sword::~Sword()
{
}