//Header Include
#include "Pellet.h"

//Component Includes
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"

//Actor Includes
#include "Player.h"
#include "Portal.h"
#include "EnergyCatcher.h"
#include "EnergyCube.h"
#include "EnergyGlass.h"

//Variable Includes
#include "Game.h"
#include "Renderer.h"

//Function Includes
#include "Mesh.h"

Pellet::Pellet(Game* game, Vector3 forward)
: Actor(game)
{
	CollisionComponent* cc = new CollisionComponent(this);
	cc->SetSize(COLLISION_DIM.x, COLLISION_DIM.y, COLLISION_DIM.z);

	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh(MESH_FILE));
	mc->SetTextureIndex(INIT_TEXTURE_INDEX);

	mForward = forward;
}

Pellet::~Pellet()
{
}

void Pellet::PelletTeleport(class Portal* entryPortal, class Portal* exitPortal)
{
	Vector3 entryFacing = entryPortal->GetNormal();
	Vector3 exitFacing = exitPortal->GetNormal();

	// Adjust Forward via Rotation
	mForward = Vector3::Normalize(exitFacing);

	// Adjust Position
	Vector3 position = Vector3::Zero;
	if (Math::Abs(entryFacing.z) == 1 || Math::Abs(exitFacing.z) == 1)
		position = exitPortal->GetPosition();
	else
		position = entryPortal->GetPortalOutVector(mPosition, exitPortal, 1);

	position += exitPortal->GetQuatForward() * 50.0f;
	mPosition = position;

	mLifetime = 0.0f;
}

void Pellet::OnUpdate(float deltaTime)
{
	if (mState == ActorState::Active)
	{
		// Move via Forward and Speed
		mPosition += mForward * (SPEED * deltaTime);

		// Deal Damage to Actors with Health
		CollisionComponent* collision = GetComponent<CollisionComponent>();
		for (Actor* collider : mGame->GetColliders())
		{
			if (collision->Intersect(collider->GetComponent<CollisionComponent>()))
			{
				HealthComponent* health = collider->GetComponent<HealthComponent>();
				if (health != nullptr && !health->IsDead())
				{
					health->TakeDamage(PELLET_DAMAGE, mPosition);
					mGame->GetAudio()->PlaySound(DESTROY_SOUND, false, this, false);
					SetState(ActorState::Destroy);
					return;
				}
			}
		}

		if (collision->Intersect(mGame->GetPlayer()->GetComponent<CollisionComponent>()))
		{
			mGame->GetPlayer()->GetComponent<HealthComponent>()->TakeDamage(PELLET_DAMAGE,
																			mPosition);
			mGame->GetAudio()->PlaySound(DESTROY_SOUND, false, this, false);
			SetState(ActorState::Destroy);
			return;
		}

		if (mLifetime > INIT_INVULNERABILITY)
		{
			// Check for Portal Teleport
			Portal* bluePortal = mGame->GetBluePortal();
			Portal* orangePortal = mGame->GetOrangePortal();
			if (bluePortal != nullptr && orangePortal != nullptr)
			{
				if (collision->Intersect(bluePortal->GetComponent<CollisionComponent>()))
				{
					PelletTeleport(bluePortal, orangePortal);
					return;
				}
				if (collision->Intersect(orangePortal->GetComponent<CollisionComponent>()))
				{
					PelletTeleport(orangePortal, bluePortal);
					return;
				}
			}

			// After 0.25s, destruction on collision
			for (Actor* collider : mGame->GetColliders())
			{
				if (collision->Intersect(collider->GetComponent<CollisionComponent>()))
				{
					EnergyCatcher* energyCatcher = dynamic_cast<EnergyCatcher*>(collider);
					EnergyCube* energyCube = dynamic_cast<EnergyCube*>(collider);
					EnergyGlass* energyGlass = dynamic_cast<EnergyGlass*>(collider);
					if (energyCatcher != nullptr && !energyCatcher->IsActivated())
					{
						mPosition = energyCatcher->GetPosition() +
									(energyCatcher->GetForward() * PELLET_CATCH_DIST);
						energyCatcher->Activate();
						SetState(ActorState::Paused);
						return;
					}
					else if (energyCube != nullptr)
					{
						GetComponent<MeshComponent>()->SetTextureIndex(ENERGY_TEXTURE_INDEX);
						mIsEnergized = true;
					}
					else if (energyGlass == nullptr || !mIsEnergized)
					{
						mGame->GetAudio()->PlaySound(DESTROY_SOUND, false, this, false);
						SetState(ActorState::Destroy);
						return;
					}
				}
			}
		}
		else
			mLifetime += deltaTime;
	}
}
