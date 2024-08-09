#include "Player.h"
#include "Game.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"
#include <string>

Player::Player(Game* game)
: Actor(game)
{
	mCollision = new CollisionComponent(this);
	mCollision->SetSize(PLAYER_WIDTH, PLAYER_HEIGHT);

	mSprite = new AnimatedSprite(this, DRAW_LAYER);
	mSprite->LoadAnimations("Assets/Link");
	mSprite->SetAnimation("StandDown");

	mMove = new PlayerMove(this);
}

Player::~Player()
{
}