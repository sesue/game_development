#include "Portal.h"
#include "Game.h"
#include "Actor.h"
#include "Player.h"
#include "PortalMeshComponent.h"
#include "CameraComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"

Portal::Portal(class Game* game, bool isBlue, Vector3 portalNormal)
: Actor(game)
, mNormal(portalNormal)
{
	mIsBlue = isBlue;
	PortalMeshComponent* portalMesh = new PortalMeshComponent(this);
	portalMesh->SetTextureIndex((isBlue ? 0 : 1));
	MeshComponent* mesh = new MeshComponent(this, true);
	mesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Meshes/Portal.gpmesh"));
	mesh->SetTextureIndex(isBlue ? 2 : 3);

	CollisionComponent* cc = new CollisionComponent(this);
	if (Math::Abs(portalNormal.x) == 1)
		cc->SetSize(WIDTH, HEIGHT, DEPTH);
	else if (Math::Abs(portalNormal.y) == 1)
		cc->SetSize(DEPTH, HEIGHT, WIDTH);
	else
		cc->SetSize(WIDTH, DEPTH, HEIGHT);
}

void Portal::OnUpdate(float deltaTime)
{
	if (mIsBlue)
		CalcViewMatrix(GetGame()->GetRenderer()->GetBluePortal(), GetGame()->GetOrangePortal());
	else
		CalcViewMatrix(GetGame()->GetRenderer()->GetOrangePortal(), GetGame()->GetBluePortal());
}

Vector3 Portal::GetPortalOutVector(const Vector3& initialVector, Portal* exitPortal, float wComp)
{
	Matrix4 inverseWT = GetWorldTransform();
	inverseWT.Invert();
	Matrix4 exitPortalWT = exitPortal->GetWorldTransform();

	Vector3 vectorObj = Vector3::Transform(initialVector, inverseWT, wComp);
	Vector3 exitVectorObj = Vector3::Transform(vectorObj, Matrix4::CreateRotationZ(Math::Pi),
											   wComp);
	Vector3 exitVector = Vector3::Transform(exitVectorObj, exitPortalWT, wComp);

	return exitVector;
}

void Portal::CalcViewMatrix(struct PortalData& portalData, Portal* exitPortal)
{
	if (exitPortal == nullptr)
	{
		portalData.mView = Matrix4::CreateScale(0.0f);
		return;
	}

	Player* player = mGame->GetPlayer();

	portalData.mCameraPos = GetPortalOutVector(player->GetPosition(), exitPortal, 1.0f);
	portalData.mCameraForward = GetPortalOutVector(
		player->GetComponent<CameraComponent>()->GetCameraForward(), exitPortal, 0.0f);
	portalData.mCameraUp = GetWorldTransform().GetZAxis();

	portalData.mView = Matrix4::CreateLookAt(
		portalData.mCameraPos, portalData.mCameraPos + portalData.mCameraForward * 50.0f,
		portalData.mCameraUp);
}
