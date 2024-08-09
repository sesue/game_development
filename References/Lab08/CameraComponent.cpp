#include "CameraComponent.h"

#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

CameraComponent::CameraComponent(class Actor* owner)
: Component(owner)
{
	SnapToIdeal();
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::SnapToIdeal()
{
	Vector3 ownerForward = mOwner->GetForward();

	//Calculate Camera Position
	mCameraPosition = IdealPosition(ownerForward);
	mCameraPosition.z = VERTICAL_DISTANCE;

	//Calculate Target Position
	Vector3 targetPos = mOwner->GetPosition() + (ownerForward * TARGET_DISTANCE);

	//Set Render View from Camera Position and Target Position
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(
		Matrix4::CreateLookAt(mCameraPosition, targetPos, Vector3::UnitZ));
}

Vector3 CameraComponent::IdealPosition(Vector3 ownerForward)
{
	return (mOwner->GetPosition() - (ownerForward * HORIZONTAL_DISTANCE) +
			(Vector3::UnitZ * VERTICAL_DISTANCE));
}

void CameraComponent::Update(float deltaTime)
{
	//Get and Calculate need Variables
	Vector3 ownerForward = mOwner->GetForward();
	float dampConstant = 2.0f * Math::Sqrt(SPRING_CONSTANT);

	//Calculate Camera Position
	Vector3 displacement = mCameraPosition - IdealPosition(ownerForward);
	Vector3 springAccel = (-SPRING_CONSTANT * displacement) - (dampConstant * mCameraVelocity);
	mCameraVelocity += springAccel * deltaTime;
	mCameraPosition += mCameraVelocity * deltaTime;
	mCameraPosition.z = VERTICAL_DISTANCE;

	//Calculate Target Position
	Vector3 targetPos = mOwner->GetPosition() + (ownerForward * TARGET_DISTANCE);

	//Set Render View from Camera Position and Target Position
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(
		Matrix4::CreateLookAt(mCameraPosition, targetPos, Vector3::UnitZ));
}