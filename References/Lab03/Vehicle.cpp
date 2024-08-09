#include "Vehicle.h"
#include "Frog.h"
#include "Game.h"
#include "WrappingMove.h"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include "Component.h"
#include <string>

Vehicle::Vehicle(Game* game, char type, Vector2 direction)
: Actor(game)
{
	game->AddVehicle(this);

	std::string textureFile;

	if (type == 'A')
		textureFile = "Assets/CarA.png";
	else if (type == 'B')
		textureFile = "Assets/CarB.png";
	else if (type == 'C')
		textureFile = "Assets/CarC.png";
	else if (type == 'D')
		textureFile = "Assets/CarD.png";
	else if (type == 'T')
		textureFile = "Assets/Truck.png";

	mSprite = new SpriteComponent(this, DRAW_LAYER);
	mSprite->SetTexture(game->GetTexture(textureFile));

	mMove = new WrappingMove(this, VEHICLE_SPEED, direction);

	mCollision = new CollisionComponent(this);
	if (type == 'T')
		mCollision->SetSize(TRUCK_COLLISION_WIDTH, TRUCK_COLLISION_HEIGHT);
	else
		mCollision->SetSize(CAR_COLLISION_WIDTH, CAR_COLLISION_HEIGHT);
}

Vehicle::~Vehicle()
{
	mGame->RemoveVehicle(this);
}

void Vehicle::OnUpdate(float deltaTime)
{
	const Vector2 TO_FROG = Vector2(mGame->GetFrog()->GetPosition().x - GetPosition().x,
									mGame->GetFrog()->GetPosition().y - GetPosition().y);

	float angle = Math::Acos(Vector2::Dot(mMove->mDirection, Vector2::Normalize(TO_FROG)));

	if (angle <= ANGLE_OF_VISION)
		mMove->SetForwardSpeed(VEHICLE_SLOW_SPEED);
	else
		mMove->SetForwardSpeed(VEHICLE_SPEED);
}