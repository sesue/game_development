#pragma once

#include "Actor.h"
#include <string>

class Prop : public Actor
{
public:
	Prop(class Game* game, bool alpha, bool collision, std::string meshStr);
	~Prop();
};