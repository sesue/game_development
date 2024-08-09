#include "Spawner.h"
#include "Game.h"
#include "Actor.h"
#include "Player.h"
#include "Goomba.h"

Spawner::Spawner(Game* game)
: Actor(game)
{
}

Spawner::~Spawner()
{
}

void Spawner::OnUpdate(float deltaTime)
{
	if (GetPosition().x - mGame->GetPlayer()->GetPosition().x < SPAWN_X_DISTANCE)
	{
		Goomba* goomba = new Goomba(mGame);
		goomba->SetPosition(GetPosition());

		SetState(ActorState::Destroy);
	}
}