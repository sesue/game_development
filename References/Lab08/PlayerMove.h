#pragma once
#include "VehicleMove.h"
#include "Math.h"
#include <map>
#include "SDL2/SDL_scancode.h"
#include "AudioSystem.h"

class PlayerMove : public VehicleMove
{
public:
	PlayerMove(class Actor* owner);

	// Update the move component
	void ProcessInput(const Uint8* keyState) override;
	void Update(float deltaTime) override;
	void OnLapChange(int newLap) override;

	const int FINAL_LAP = 2;

protected:
	Vector3 mVelocity = Vector3::Zero;

	std::map<SDL_Scancode, bool> mPreviousFrameKeyboard = {{SDL_SCANCODE_SPACE, false},
														   {SDL_SCANCODE_Q, false}};
};
