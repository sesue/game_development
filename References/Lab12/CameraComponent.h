#pragma once
#include "Component.h"
#include "Math.h"

class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner);
	~CameraComponent();

	//Camera Constants
	const float TARGET_DISTANCE = 50.0f;

	void Update(float deltaTime) override;

	float GetPitchAngle() { return mPitchAngle; }
	Vector3 GetCameraForward() { return mCameraForward; }
	float GetPitchSpeed() { return mPitchSpeed; }
	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }

	void ResetPitchAngle() { mPitchAngle = 0.0f; }

private:
	Vector3 IdealPosition(Vector3 ownerForward);

	float mPitchAngle = 0.0f;
	float mPitchSpeed = 0.0f;
	Vector3 mCameraForward = Vector3::Zero;
};
