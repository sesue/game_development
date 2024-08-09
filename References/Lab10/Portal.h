#pragma once
#include "Actor.h"
#include <string>

class Portal : public Actor
{
public:
	Portal(class Game* game, bool isBlue, Vector3 normal);

	const std::string PORTAL_MESH = "Assets/Meshes/Portal.gpmesh";

	const float HEIGHT = 125.0f;
	const float WIDTH = 110.0f;
	const float DEPTH = 10.0f;

	const Vector3 GetNormal() const { return mNormal; }

	void OnUpdate(float deltaTime) override;

	Vector3 GetPortalOutVector(const Vector3& initialVector, Portal* exitPortal, float wComp);
	void CalcViewMatrix(struct PortalData& portalData, Portal* exitPortal);

	bool GetIsBlue() { return mIsBlue; }

private:
	bool mIsBlue;

	Vector3 mNormal;
};
