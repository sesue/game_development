//Header Include
#include "PlayerMove.h"

//Components
#include "CollisionComponent.h"

//Actor Include
#include "Actor.h"
#include "Enemy.h"

//Functional Include
#include "Game.h"
#include "Renderer.h"
#include "Random.h"
#include "HeightMap.h"
#include "PlayerUI.h"

PlayerMove::PlayerMove(Actor* owner)
: VehicleMove(owner)
{
	owner->SetPosition(owner->GetGame()->GetHeightMap()->CellToWorld(39, 58));
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
	if (keyState[SDL_SCANCODE_W])
		SetAccPressed(true);
	else
		SetAccPressed(false);

	if (keyState[SDL_SCANCODE_A] && !keyState[SDL_SCANCODE_D])
		SetTurn(Turning::Left);
	else if (keyState[SDL_SCANCODE_D] && !keyState[SDL_SCANCODE_A])
		SetTurn(Turning::Right);
	else
		SetTurn(Turning::None);
}

void PlayerMove::Update(float deltaTime)
{

	VehicleMove::Update(deltaTime);
}

void PlayerMove::OnLapChange(int newLap)
{
	Game* game = mOwner->GetGame();
	AudioSystem* audio = game->GetAudio();
	Enemy* enemy = game->GetEnemy();

	if (mLap == FINAL_LAP + 1)
	{
		game->GetAudio()->StopSound(game->GetSoundHandle());

		if (mLap > enemy->GetComponent<VehicleMove>()->GetLap())
		{
			mOwner->GetComponent<PlayerUI>()->SetRaceState(PlayerUI::Won);
			audio->PlaySound("Won.wav");
		}
		else
		{
			mOwner->GetComponent<PlayerUI>()->SetRaceState(PlayerUI::Lost);
			audio->PlaySound("Lost.wav");
		}

		mOwner->SetState(ActorState::Paused);
		enemy->SetState(ActorState::Paused);
	}
	else
		mOwner->GetComponent<PlayerUI>()->OnLapChange(newLap);

	if (mLap == FINAL_LAP)
	{
		audio->StopSound(game->GetSoundHandle(), 250);

		audio->PlaySound("FinalLap.wav");
		game->SetSoundHandle(audio->PlaySound("MusicFast.ogg", true, 4000));
	}
}