//Header Include
#include "Player.h"

//Component Includes
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"
#include "CameraComponent.h"
#include "HealthComponent.h"
#include "HUD.h"

//Variable Includes
#include "Game.h"
#include "Renderer.h"
#include "PlayerMesh.h"
#include "Random.h"
#include "AudioSystem.h"

//Function Includes
#include "Mesh.h"

Player::Player(Game* game, Vector3 initialPos, bool hasGun)
: Actor(game)
{
	if (hasGun)
		GiveGun();

	SetPosition(initialPos);
	mInitialPos = initialPos;

	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(PLAYER_COLLISION_SIZE.x, PLAYER_COLLISION_SIZE.y, PLAYER_COLLISION_SIZE.z);

	new CameraComponent(this);

	new PlayerMove(this);

	HealthComponent* health = new HealthComponent(this);
	health->SetOnDamage([this](const Vector3& location) {
		Vector3 direction = location - mPosition;
		direction.z = 0.0f;
		direction = Vector3::Normalize(direction);

		Vector3 forward = GetForward();
		forward.z = 0.0f;
		forward = Vector3::Normalize(forward);

		Vector3 cross = Vector3::Cross(forward, direction);
		float dot = Vector3::Dot(forward, direction);
		float angle = ((cross.z >= 0.0f) ? -1 : 1) * Math::Acos(dot);

		mHUD->PlayerTakeDamage(angle);
	});
	health->SetOnDeath([this] {
		int index = Random::GetIntRange(0, TAUNT_SOUNDS.size()-1);

		GetComponent<PlayerMove>()->SetDeathSound(mGame->GetAudio()->PlaySound(TAUNT_SOUNDS[index]));
		mHUD->ShowSubtitle(TAUNT_SUBTITLES[index]);
	});

	mHUD = new HUD(this);
}

Player::~Player()
{
}

void Player::GiveGun()
{
	mHasGun = true;

	new PlayerMesh(mGame);
}