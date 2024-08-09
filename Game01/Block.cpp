#include "Block.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include <string>

Block::Block(Game* game, char type)
: Actor(game)
{
	mCollision = new CollisionComponent(this);
	mCollision->SetSize(BLOCK_WIDTH, BLOCK_HEIGHT);

	mSprite = new SpriteComponent(this, DRAW_LAYER);
	std::string textureFile = "Assets/Block.png";
	textureFile.insert(12, 1, type);
	mSprite->SetTexture(game->GetTexture(textureFile));

	game->AddBlock(this);
}

Block::~Block()
{
	mGame->RemoveBlock(this);
}
