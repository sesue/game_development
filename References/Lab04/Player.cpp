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

	std::vector<SDL_Texture*> idleAnim{GetGame()->GetTexture("Assets/Mario/Idle.png")};
	mSprite->AddAnimation("idle", idleAnim);

	std::vector<SDL_Texture*> deadAnim{GetGame()->GetTexture("Assets/Mario/Dead.png")};
	mSprite->AddAnimation("dead", deadAnim);

	std::vector<SDL_Texture*> jumpLeftAnim{GetGame()->GetTexture("Assets/Mario/JumpLeft.png")};
	mSprite->AddAnimation("jumpLeft", jumpLeftAnim);

	std::vector<SDL_Texture*> jumpRightAnim{GetGame()->GetTexture("Assets/Mario/JumpRight.png")};
	mSprite->AddAnimation("jumpRight", jumpRightAnim);

	std::vector<SDL_Texture*> runLeftAnim{GetGame()->GetTexture("Assets/Mario/RunLeft0.png"),
										  GetGame()->GetTexture("Assets/Mario/RunLeft1.png"),
										  GetGame()->GetTexture("Assets/Mario/RunLeft2.png")};
	mSprite->AddAnimation("runLeft", runLeftAnim);

	std::vector<SDL_Texture*> runRightAnim{GetGame()->GetTexture("Assets/Mario/RunRight0.png"),
										   GetGame()->GetTexture("Assets/Mario/RunRight1.png"),
										   GetGame()->GetTexture("Assets/Mario/RunRight2.png")};
	mSprite->AddAnimation("runRight", runRightAnim);

	mSprite->SetAnimation("idle");

	mMove = new PlayerMove(this);
}

Player::~Player()
{
}