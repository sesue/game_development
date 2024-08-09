#include "PlayerUI.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Actor.h"
#include "Enemy.h"
#include "VehicleMove.h"
#include "Player.h"
#include "SDL2/SDL.h"

PlayerUI::PlayerUI(Actor* owner)
: UIComponent(owner)
{
	Renderer* r = owner->GetGame()->GetRenderer();
	mLapTextures = std::vector<Texture*>{
		r->GetTexture("Assets/UI/Lap1.png"),
		r->GetTexture("Assets/UI/FinalLap.png"),
	};

	mFirstTexture = r->GetTexture("Assets/UI/1st.png");
	mSecondTexture = r->GetTexture("Assets/UI/2nd.png");

	mGoTexture = r->GetTexture("Assets/UI/Go.png");
	mReadyTexture = r->GetTexture("Assets/UI/Ready.png");
}

void PlayerUI::Update(float deltaTime)
{
	mGoDisplayTimer -= deltaTime;
	mLapDisplayTimer -= deltaTime;
}

void PlayerUI::Draw(Shader* shader)
{
	if (mGoDisplayTimer > 0.0f)
	{
		if (mOwner->GetState() == ActorState::Paused)
		{
			DrawTexture(shader, mReadyTexture, Vector2(0.0f, 100.0f));
		}
		else if (mOwner->GetState() == ActorState::Active)
		{
			DrawTexture(shader, mGoTexture, Vector2(0.0f, 100.0f));
		}
	}

	if (mLapDisplayTimer > 0.0f)
	{
		DrawTexture(shader, mLapTextures[mLapIndex], Vector2(0.0f, 200.0f), 0.75f);
	}

	if (mRaceState == Won)
	{
		DrawTexture(shader, mFirstTexture, Vector2(0.0f, 100.0f));
	}
	else if (mRaceState == Lost)
	{
		DrawTexture(shader, mSecondTexture, Vector2(0.0f, 100.0f));
	}

	// Figure out what place to show
	if (mOwner->GetState() == ActorState::Active)
	{
		bool inFirst = IsPlayerInFirst();
		if (inFirst)
		{
			DrawTexture(shader, mFirstTexture, Vector2(400.0f, 320.0f), 0.5f);
		}
		else
		{
			DrawTexture(shader, mSecondTexture, Vector2(400.0f, 320.0f), 0.5f);
		}
	}
}

void PlayerUI::OnLapChange(int lapNum)
{
	mLapIndex = lapNum - 1;
	mLapDisplayTimer = 3.0f;
}

bool PlayerUI::IsPlayerInFirst() const
{
	VehicleMove* playerVehicle = mOwner->GetGame()->GetPlayer()->GetComponent<VehicleMove>();
	VehicleMove* enemyVehicle = mOwner->GetGame()->GetEnemy()->GetComponent<VehicleMove>();

	//Player is a lap ahead
	if (playerVehicle->GetLap() > enemyVehicle->GetLap())
		return true;
	//Player is checkpoint ahead
	else if (playerVehicle->GetNextCheckpoint() > enemyVehicle->GetNextCheckpoint() &&
			 enemyVehicle->GetNextCheckpoint() != 0 &&
			 playerVehicle->GetLap() == enemyVehicle->GetLap())
		return true;
	else if (playerVehicle->GetNextCheckpoint() != 0 && enemyVehicle->GetNextCheckpoint() == 0 &&
			 playerVehicle->GetLap() == enemyVehicle->GetLap())
		return false;
	else if (playerVehicle->GetNextCheckpoint() == 0 && enemyVehicle->GetNextCheckpoint() != 0 &&
			 playerVehicle->GetLap() == enemyVehicle->GetLap())
		return true;
	//Player is closer to next checkpoint
	else if (playerVehicle->GetDistToNextCheckpoint() < enemyVehicle->GetDistToNextCheckpoint() &&
			 playerVehicle->GetNextCheckpoint() == enemyVehicle->GetNextCheckpoint() &&
			 playerVehicle->GetLap() == enemyVehicle->GetLap())
		return true;

	return false;
}
