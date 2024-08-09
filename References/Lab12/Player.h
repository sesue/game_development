#pragma once

#include "Actor.h"
#include <string>

class Player : public Actor
{
public:
	Player(class Game* game, Vector3 initialPos, bool hasGun);
	~Player();

	const Vector3 PLAYER_COLLISION_SIZE = Vector3(50.0f, 100.0f, 50.0f);

	const bool HasGun() const { return mHasGun; }
	void GiveGun();

	Vector3 mInitialPos;

	class HUD* GetHUD() { return mHUD; }

	const std::vector<std::string> TAUNT_SOUNDS = {
		"Glados-PlayerDead1.ogg", 
		"Glados-PlayerDead2.ogg", 
		"Glados-PlayerDead3.ogg",
		"Glados-PlayerDead4.ogg"};
	const std::vector<std::string> TAUNT_SUBTITLES = {
		"Congratulations! The test is now over.",
		"Thank you for participating in this Aperture Science computer-aided enrichment activity.",
		"Goodbye.", 
		"You're not a good person. You know that, right?"};

private:
	bool mHasGun = false;

	class HUD* mHUD = nullptr;
};