#include "CameraComponent.h"

#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

CameraComponent::CameraComponent(class Actor* owner)
: Component(owner)
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Update(float deltaTime)
{
	mPitchAngle = Math::Clamp(mPitchAngle + (mPitchSpeed * deltaTime), -Math::Pi / 2.1f,
							  Math::Pi / 2.1f);

	//Calculate Camera Forward
	Matrix4 pitchMatrix = Matrix4::CreateRotationY(mPitchAngle);
	Matrix4 yawMatrix = Matrix4::CreateRotationZ(mOwner->GetRotation());
	Matrix4 rotationMatrix = pitchMatrix * yawMatrix;
	mCameraForward = Vector3::Transform(Vector3::UnitX, rotationMatrix);

	//Get and Calculate need Variables
	Vector3 ownerPos = mOwner->GetPosition();
	Vector3 targetPos = ownerPos + (mCameraForward * TARGET_DISTANCE);

	//Set Render View from Camera Position and Target Position
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(
		Matrix4::CreateLookAt(ownerPos, targetPos, Vector3::UnitZ));
}