#include "Effect.h"
#include "AnimatedSprite.h"
#include "AudioSystem.h"
#include "Game.h"

Effect::Effect(Game* game, Vector2 position, std::string animationName, std::string soundName)
: Actor(game)
{
	mPosition = position;
	AnimatedSprite* as = new AnimatedSprite(this);
	as->LoadAnimations(EFFECT_ANIMATIONS);
	as->SetAnimation(animationName);
	mLifeTime = as->GetAnimDuration(animationName);

	game->GetAudio()->PlaySound(soundName);
}

Effect::~Effect()
{
}

void Effect::OnUpdate(float deltaTime)
{
	if (mLifeTime <= 0.0f)
		SetState(ActorState::Destroy);
	else
		mLifeTime -= deltaTime;
}