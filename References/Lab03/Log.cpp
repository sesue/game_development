#include "Log.h"
#include "Game.h"
#include "WrappingMove.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include <string>

Log::Log(Game* game, char type, Vector2 direction)
: Actor(game)
{
	std::string textureFile;
	mCollision = new CollisionComponent(this);

	if (type == 'X')
	{
		mCollision->SetSize(X_LOG_WIDTH, LOG_HEIGHT);
		textureFile = "Assets/LogX.png";
	}
	else if (type == 'Y')
	{
		mCollision->SetSize(Y_LOG_WIDTH, LOG_HEIGHT);
		textureFile = "Assets/LogY.png";
	}
	else if (type == 'Z')
	{
		mCollision->SetSize(Z_LOG_WIDTH, LOG_HEIGHT);
		textureFile = "Assets/LogZ.png";
	}

	mSprite = new SpriteComponent(this, DRAW_LAYER);
	mSprite->SetTexture(game->GetTexture(textureFile));

	mMove = new WrappingMove(this, LOG_SPEED, direction);

	game->AddLog(this);
}

Log::~Log()
{
	mGame->RemoveLog(this);
}
