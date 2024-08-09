#include "Soldier.h"
#include "SoldierAI.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "EnemyComponent.h"
#include "PathFinder.h"
#include "PathNode.h"
#include "Game.h"
#include "Effect.h"

Soldier::Soldier(Game* game, PathNode* start, PathNode* end)
: Actor(game)
{
	CollisionComponent* collision = new CollisionComponent(this);
	collision->SetSize(SOLDIER_WIDTH, SOLDIER_HEIGHT);

	AnimatedSprite* animatedSprite = new AnimatedSprite(this);
	animatedSprite->LoadAnimations(SOLDIER_ASSETS);
	animatedSprite->SetAnimation("WalkDown");
	animatedSprite->SetAnimFPS(ANIMATION_FPS);

	SoldierAI* ai = new SoldierAI(this);
	ai->Setup(start, end);

	EnemyComponent* enemy = new EnemyComponent(this, SOLDIER_HEALTH);
	enemy->SetOnDamage([this] {
		GetComponent<SoldierAI>()->Stun();
		new Effect(GetGame(), mPosition, "Hit", "EnemyHit.wav");
	});
	enemy->SetOnDeath([this] {
		new Effect(GetGame(), mPosition, "Death", "EnemyDie.wav");
	});
}

Soldier::~Soldier()
{
}