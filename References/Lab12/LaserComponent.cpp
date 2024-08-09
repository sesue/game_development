//Header Include
#include "LaserComponent.h"

//Actor Include
#include "Actor.h"
#include "Portal.h"

//Component Include
#include "HealthComponent.h"

//Utility Include
#include "Shader.h"
#include "Mesh.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"


LaserComponent::LaserComponent(Actor* owner)
: MeshComponent(owner, false)
{
	SetMesh(owner->GetGame()->GetRenderer()->GetMesh(MESH_FILE));
}

LaserComponent::~LaserComponent()
{

}


Matrix4 LaserComponent::TransformLineSeg(LineSegment& line)
{
	Matrix4 scale = Matrix4::CreateScale(line.Length(), 1.0f, 1.0f);
	Matrix4 rotation = Matrix4::CreateFromQuaternion(mOwner->GetGame()->CreateArbitraryQuaternion(Vector3::UnitX, Vector3::Normalize(line.mEnd-line.mStart)));
	Matrix4 translate = Matrix4::CreateTranslation(line.PointOnSegment(0.5f));

	return (scale * rotation * translate);
}

void LaserComponent::Update(float deltaTime)
{
	Game* game = mOwner->GetGame();

	mLasers.clear();

	if (mActive)
	{
		CastInfo outInfo;
		Vector3 startPoint = mOwner->GetWorldPosition();
		Vector3 farPoint = startPoint + (mOwner->GetWorldForward() * LASER_MAX_DIST);
		Actor* ignoreActor = mTurret;
		for (int i = 0; i < 2; i++)
		{
			mLasers.push_back(LineSegment(startPoint, farPoint));

			if (SegmentCast(game->GetActors(), mLasers.back(), outInfo, ignoreActor))
			{
				mLasers.back().mEnd = outInfo.mPoint;

				Portal* entryPortal = dynamic_cast<Portal*>(outInfo.mActor);
				if (entryPortal != nullptr)
				{
					Portal* exitPortal = entryPortal->GetIsBlue() ? game->GetOrangePortal()
																  : game->GetBluePortal();

					if (exitPortal != nullptr)
					{
						Vector3 forwardVec = entryPortal->GetPortalOutVector(
							Vector3::Normalize(mLasers[0].mEnd - mLasers[0].mStart), exitPortal, 0);
						startPoint = entryPortal->GetPortalOutVector(mLasers[0].mEnd, exitPortal,
																	 1) +
									 (forwardVec * PORTAL_EXIT_BUMP);
						farPoint = startPoint + (forwardVec * LASER_MAX_DIST);
						ignoreActor = exitPortal;
					}
				}
			}
			else
				break;
		}

		mLastHitActor = outInfo.mActor;
		HealthComponent* health = nullptr;
		if (mLastHitActor != nullptr)
			health = mLastHitActor->GetComponent<HealthComponent>();
		if (dynamic_cast<Portal*>(outInfo.mActor) != nullptr ||
			(health != nullptr && health->IsDead()))
			mLastHitActor = nullptr;
	}
}

void LaserComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		for (LineSegment line : mLasers)
		{
			// Set the world transform
			shader->SetMatrixUniform("uWorldTransform", TransformLineSeg(line));
			// Set the active texture
			Texture* t = mMesh->GetTexture(mTextureIndex);
			if (t)
			{
				t->SetActive();
			}
			// Set the mesh's vertex array as active
			VertexArray* va = mMesh->GetVertexArray();
			va->SetActive();
			// Draw
			glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
		}
	}
}
