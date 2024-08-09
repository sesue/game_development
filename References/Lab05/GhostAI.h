#pragma once
#include "Component.h"
#include "Math.h"
#include <vector>

class GhostAI : public Component
{
public:
	// Used to track the four different GhostAI states
	enum State
	{
		Scatter,
		Chase,
		Frightened,
		Dead
	};

	GhostAI(class Actor* owner);

	void Update(float deltaTime) override;

	// Called when the Ghost starts at the beginning
	// (or when the ghosts should respawn)
	void Start(class PathNode* startNode);

	// Get the current state
	State GetState() const { return mState; }

	// Called when the ghost should switch to the "Frightened" state
	void Frighten();

	// Called when the ghost should switch to the "Dead" state
	void Die();

	//  Helper function to draw GhostAI's current goal, prev, and next
	void DebugDrawPath(struct SDL_Renderer* render);

private:
	// Member data for pathfinding

	// TargetNode is our current goal node
	class PathNode* mTargetNode = nullptr;
	// PrevNode is the last node we intersected
	// with prior to the current position
	class PathNode* mPrevNode = nullptr;
	// NextNode is the next node we're trying to get to
	class PathNode* mNextNode = nullptr;

	// Current state of the Ghost AI
	State mState = Scatter;

	// Save the owning actor (cast to a Ghost*)
	class Ghost* mGhost;

	// Add any member data/helper functions here!
	//State Change data/functions
	const float SCATTER_TIME = 5.0f;
	const float CHASE_TIME = 20.0f;
	const float FRIGHTENED_0_TIME = 5.0f;
	const float FRIGHTENED_1_TIME = 2.0f;
	float mStateTime = 0.0f;
	void StateChange();

	//Movement data/functions
	const float NORMAL_SPEED = 90.0f;
	const float FRIGHT_SPEED = 65.0f;
	const float DEAD_SPEED = 125.0f;

	float mCurrentSpeed = NORMAL_SPEED;
	Vector2 mDirection = Vector2::Zero;

	Vector2 CalcDirection();
	void UpdateMovementNodes();

	//Updateing Target Node
	void UpdateTargetNode();
	void FrightenedTargetNode();
	void TargetClosestDefault(Vector2 targetPosition);
	void BlinkyTargetNode();
	void PinkyTargetNode();
	void InkyTargetNode();
	void ClydeTargetNode();

	const float PINKY_TARGET_DISTANCE = 80.0f;
	const float INKY_TARGET_DISTANCE = 40.0f;
	const float CLYDE_TARGET_THRESHOLD = 150.0f;
};
