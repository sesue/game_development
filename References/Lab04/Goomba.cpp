#include "Goomba.h"
#include "Game.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "GoombaMove.h"

Goomba::Goomba(Game* game)
: Actor(game)
{
	mSprite = new AnimatedSprite(this, Goomba::DRAW_LAYER);
	std::vector<SDL_Texture*> walkAnim{GetGame()->GetTexture("Assets/Goomba/Walk0.png"),
									   GetGame()->GetTexture("Assets/Goomba/Walk1.png")};
	mSprite->AddAnimation("walk", walkAnim);
	std::vector<SDL_Texture*> deadAnim{GetGame()->GetTexture("Assets/Goomba/Dead.png")};
	mSprite->AddAnimation("dead", deadAnim);
	mSprite->SetAnimation("walk");

	mCollision = new CollisionComponent(this);
	mCollision->SetSize(GOOMBA_WIDTH, GOOMBA_HEIGHT);

	mMove = new GoombaMove(this, mCollision);

	game->AddEnemy(this);
}

Goomba::~Goomba()
{
	mGame->RemoveEnemy(this);
}

void Goomba::OnUpdate(float deltaTime)
{
	if (mLifeTime <= 0.0f)
		SetState(ActorState::Destroy);
	else if (mStomped)
		mLifeTime -= deltaTime;
}

void Goomba::Stomp()
{
	mStomped = true;
	mSprite->SetAnimation("dead");
	mGame->RemoveEnemy(this);
}