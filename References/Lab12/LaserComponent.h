#pragma once
#include "MeshComponent.h"
#include "SegmentCast.h"
#include <vector>
#include <string>

class LaserComponent : public MeshComponent
{
public:
	LaserComponent(class Actor* owner);
	~LaserComponent();

	const std::string MESH_FILE = "Assets/Meshes/Laser.gpmesh";

	void Update(float deltaTime) override;
	void Draw(class Shader* shader) override;

	const float LASER_MAX_DIST = 350.0f;
	const float PORTAL_EXIT_BUMP = 5.5f;

	void SetTurret(class Actor* turret) { mTurret = turret; }
	void TurretDied() { mActive = false; }

	class Actor* GetLastHitActor() { return mLastHitActor; }

private:
	std::vector<LineSegment> mLasers;

	class Matrix4 TransformLineSeg(class LineSegment& line);

	class Actor* mTurret = nullptr;

	class Actor* mLastHitActor = nullptr;

	bool mActive = true;
};