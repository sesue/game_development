#include "Bush.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "EnemyComponent.h"
#include "Game.h"
#include "PathFinder.h"
#include "Effect.h"

Bush::Bush(Game* game)
: Actor(game)
{
	SpriteComponent* sprite = new SpriteComponent(this);
	sprite->SetTexture(game->GetTexture(BUSH_ASSET));

	CollisionComponent* collision = new CollisionComponent(this);
	collision->SetSize(BUSH_WIDTH, BUSH_HEIGHT);

	EnemyComponent* enemy = new EnemyComponent(this, BUSH_HEALTH);
	enemy->SetOnDeath([this] {
		GetGame()->GetPaths()->SetIsBlocked(static_cast<int>(mPosition.y) / 32,
											static_cast<int>(mPosition.x) / 32, false);
		new Effect(GetGame(), mPosition, "BushDeath", "BushDie.wav");
	});
}

Bush::~Bush()
{
}