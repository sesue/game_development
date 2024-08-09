//Header Include
#include "TurretHead.h"

//Component Includes
#include "MeshComponent.h"
#include "LaserComponent.h"
#include "HealthComponent.h"
#include "CollisionComponent.h"

//Actor Includes
#include "Player.h"
#include "Portal.h"
#include "TurretBase.h"

//Variable Includes
#include "Game.h"
#include "Renderer.h"
#include "Random.h"

//Function Includes
#include "Mesh.h"

TurretHead::TurretHead(Game* game, Actor* parent)
: Actor(game, parent)
{
	mPosition = INIT_POSITION;
	SetScale(INIT_SCALE);

	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh(MESH_FILE));

	mLaserActor = new Actor(game, this);
	mLaserActor->SetPosition(LASER_POS);
	mLaser = new LaserComponent(mLaserActor);
	mLaser->SetTurret(parent);
}

TurretHead::~TurretHead()
{
}

void TurretHead::OnUpdate(float deltaTime)
{
	mStateTimer += deltaTime;
	mTeleportTimer += deltaTime;

	switch (mCurrState)
	{
	case TurretState::Idle:
		UpdateIdle(deltaTime);
		break;
	case TurretState::Search:
		UpdateSearch(deltaTime);
		break;
	case TurretState::Priming:
		UpdatePriming(deltaTime);
		break;
	case TurretState::Firing:
		UpdateFiring(deltaTime);
		break;
	case TurretState::Falling:
		UpdateFalling(deltaTime);
		break;
	case TurretState::Dead:
		UpdateDead(deltaTime);
		break;
	}
}

void TurretHead::UpdateIdle(float deltaTime)
{
	if (CheckForTeleport())
	{
		ChangeState(TurretState::Falling);
		return;
	}

	CheckForTarget();
	if (mTarget != nullptr)
		ChangeState(TurretState::Priming);
}

bool EqualQuat(Quaternion a, Quaternion b)
{
	return !((a.x != b.x) || (a.y != b.y) || (a.z != b.z) || (a.w != b.w));
}

void TurretHead::UpdateSearch(float deltaTime)
{
	if (CheckForTeleport())
	{
		ChangeState(TurretState::Falling);
		return;
	}

	CheckForTarget();
	if (mTarget != nullptr)
		ChangeState(TurretState::Priming);
	else
	{
		if (!EqualQuat(mQuat, mEndQuat))
		{
			mMoveTimer += deltaTime;
			if (mMoveTimer >= MOVE_TIME)
				SetQuat(mEndQuat);
			else
				SetQuat(Quaternion::Slerp(mStartQuat, mEndQuat,
										  Math::Clamp(mMoveTimer / MOVE_TIME, 0.0f, 1.0f)));
		}
		else if (EqualQuat(mEndQuat, Quaternion::Identity))
		{
			if (mStateTimer > SEARCH_TIMEOUT)
			{
				ChangeState(TurretState::Idle);
			}
			else
			{
				float angle = Random::GetFloatRange(-Math::Pi, Math::Pi);
				Vector3 unitOffset = Vector3(0.0f, SIDE_DIST * Math::Cos(angle),
											 UP_DIST * Math::Sin(angle));
				mEndQuat = mGame->CreateArbitraryQuaternion(
					Vector3::UnitX, (Vector3::UnitX * FWD_DIST) + unitOffset);
				mStartQuat = Quaternion::Identity;
				mMoveTimer = 0.0f;
			}
		}
		else
		{
			mStartQuat = mEndQuat;
			mEndQuat = Quaternion::Identity;
			mMoveTimer = 0.0f;
		}
	}

}

void TurretHead::UpdatePriming(float deltaTime)
{
	if (CheckForTeleport())
	{
		ChangeState(TurretState::Falling);
		return;
	}

	if (mTarget != mLaser->GetLastHitActor())
		ChangeState(TurretState::Search);
	else if (mStateTimer > PRIMING_TIMEOUT)
		ChangeState(TurretState::Firing);
}

void TurretHead::UpdateFiring(float deltaTime)
{
	if (CheckForTeleport())
	{
		ChangeState(TurretState::Falling);
		return;
	}

	if (mTarget != mLaser->GetLastHitActor())
		ChangeState(TurretState::Search);

	if (mFireTimer > FIRING_COOLDOWN)
	{
		HealthComponent* health = mTarget->GetComponent<HealthComponent>();
		if (health != nullptr && !health->IsDead())
		{
			health->TakeDamage(TURRET_DAMAGE, GetWorldPosition());
			mGame->GetAudio()->PlaySound(BULLET_HIT, false, mTarget);
			mFireTimer = 0.0f;
		}
	}
	else
		mFireTimer += deltaTime;
}

void TurretHead::UpdateFalling(float deltaTime)
{
	Actor* parent = GetParent();
	Vector3 position = parent->GetPosition();

	mVelocity += GRAVITY * deltaTime;
	if (mVelocity.Length() > TERMINAL_VELOCITY)
		mVelocity = Vector3::Normalize(mVelocity) * TERMINAL_VELOCITY;

	position += mVelocity * deltaTime;
	parent->SetPosition(position);
	
	if (!CheckForTeleport())
	{

		CollisionComponent* selfCollider = parent->GetComponent<CollisionComponent>();
		for (Actor* actor : mGame->GetColliders())
		{
			if (actor != parent)
			{
				CollisionComponent* collider = actor->GetComponent<CollisionComponent>();

				Vector3 offset = Vector3::Zero;
				CollSide side = selfCollider->GetMinOverlap(collider, offset);

				if (side != CollSide::None)
					position = position + offset;

				if (side == CollSide::Top && mVelocity.z < 0)
				{
					position.z -= LANDING_CORRECTION;
					Die();
					TurretBase* turret = dynamic_cast<TurretBase*>(actor);
					if (turret != nullptr)
					{
						position.z -= selfCollider->GetHeight() / 2.0f;
						turret->Die();
					}
				}

				if (side == CollSide::Top || side == CollSide::Bottom && dynamic_cast<Portal*>(actor))
					mVelocity.z = 0.0f;
			}
		}

		parent->SetPosition(position);
	}
}

void TurretHead::UpdateDead(float deltaTime)
{
	if (CheckForTeleport())
	{
		ChangeState(TurretState::Falling);
		return;
	}
}

void TurretHead::ChangeState(TurretState state)
{
	if (state != mCurrState)
	{
		mCurrState = state;
		mStateTimer = 0.0f;
		mFireTimer = 0.0f;

		AudioSystem* audio = mGame->GetAudio();
		if (audio->GetSoundState(mSoundHandle) == SoundState::Playing)
			audio->StopSound(mSoundHandle);

		mSoundHandle = audio->PlaySound(STATE_SOUNDS.at(state), false, this);
	}
}

void TurretHead::CheckForTarget()
{
	Actor* hit = mLaser->GetLastHitActor();
	if (hit != nullptr && hit->GetComponent<HealthComponent>() != nullptr)
		mTarget = hit;
	else
		mTarget = nullptr;
}

bool TurretHead::CheckForTeleport()
{
	if (mTeleportTimer > PORTAL_COOLDOWN)
	{
		Portal* bluePortal = mGame->GetBluePortal();
		Portal* orangePortal = mGame->GetOrangePortal();
		Portal* exitPortal = nullptr;
		Actor* parent = GetParent();
		if (bluePortal != nullptr && orangePortal != nullptr)
		{
			CollisionComponent* turretCol = parent->GetComponent<CollisionComponent>();
			if (turretCol->Intersect(bluePortal->GetComponent<CollisionComponent>()))
				exitPortal = orangePortal;
			else if (turretCol->Intersect(orangePortal->GetComponent<CollisionComponent>()))
				exitPortal = bluePortal;

			if (exitPortal != nullptr)
			{
				parent->SetPosition(exitPortal->GetPosition());
				mVelocity += exitPortal->GetQuatForward() * EXIT_MAGNITUDE;
				mTeleportTimer = 0.0f;
				return true;
			}
			else
				return false;
		}
	}

	return false;
}

void TurretHead::TakeDamage()
{
	if (!mDamaged)
	{
		AudioSystem* audio = mGame->GetAudio();
		if (audio->GetSoundState(mSoundHandle) == SoundState::Playing)
			audio->StopSound(mSoundHandle);

		audio->PlaySound(FRIENDLY_FIRE, false, this);
		mDamaged = true;
	}
}

void TurretHead::Die()
{
	ChangeState(TurretState::Dead);
	GetParent()->SetQuat(mGame->CreateArbitraryQuaternion(Vector3::UnitZ, Vector3::NegUnitY));
	mLaser->TurretDied();
}