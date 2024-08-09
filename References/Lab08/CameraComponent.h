#pragma once
#include "Component.h"
#include "Math.h"

class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner);
	~CameraComponent();

	//Camera Constants
	const float HORIZONTAL_DISTANCE = 60.0f;
	const float VERTICAL_DISTANCE = 70.0f;
	const float TARGET_DISTANCE = 50.0f;
	const float SPRING_CONSTANT = 256.0f;

	void Update(float deltaTime) override;

private:
	Vector3 IdealPosition(Vector3 ownerForward);
	void SnapToIdeal();

	Vector3 mCameraPosition = Vector3::Zero;
	Vector3 mCameraVelocity = Vector3::Zero;
};
