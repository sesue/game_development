#include "Actor.h"
#include "Game.h"
#include "AudioSystem.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game, Actor* parent)
: mGame(game)
, mState(ActorState::Active)
, mPosition(Vector3::Zero)
, mScale(1.0f)
, mRotation(0.0f)
, mRotationX(0.0f)
, mParent(parent)
{
	if (parent == nullptr)
		mGame->AddActor(this);
	else
		mParent->AddChild(this);
}

Actor::~Actor()
{
	mGame->GetAudio()->RemoveActor(this);

	for (Actor* childPtr : mChildren)
	{
		delete childPtr;
	}
	mChildren.clear();

	if (mParent == nullptr)
		mGame->RemoveActor(this);
	else
		mParent->RemoveChild(this);


	for (Component* componentPtr : mComponents)
	{
		delete componentPtr;
	}
	mComponents.clear();
}

void Actor::Update(float deltaTime)
{
	CalcWorldTransform();

	if (mState == ActorState::Active)
	{
		for (class Component* compPtr : mComponents)
		{
			compPtr->Update(deltaTime);
		}
	}

	OnUpdate(deltaTime);
	CalcWorldTransform();

	for (Actor* childPtr : mChildren)
	{
		childPtr->Update(deltaTime);
	}
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState, Uint32 mouseButtons, const Vector2& relativeMouse)
{
	if (mState == ActorState::Active)
	{
		for (class Component* compPtr : mComponents)
		{
			compPtr->ProcessInput(keyState, mouseButtons, relativeMouse);
		}
		OnProcessInput(keyState, mouseButtons, relativeMouse);
	}
}

void Actor::OnProcessInput(const Uint8* keyState, Uint32 mouseButtons, const Vector2& relativeMouse)
{
}

void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}

void Actor::CalcWorldTransform()
{
	Matrix4 scaleMatrix = Matrix4::CreateScale(mScale);
	Matrix4 rotationMatrix = Matrix4::CreateRotationZ(mRotation);
	Matrix4 rotationXMatrix = Matrix4::CreateRotationX(mRotationX);
	Matrix4 quaternionMatrix = Matrix4::CreateFromQuaternion(mQuat);
	Matrix4 translationMatrix = Matrix4::CreateTranslation(mPosition);

	mWorldTransform = scaleMatrix * rotationMatrix * rotationXMatrix * quaternionMatrix *
					  translationMatrix;

	if (mParent != nullptr)
	{
		if (mInheritScale)
			mWorldTransform *= mParent->GetWorldTransform();
		else
			mWorldTransform *= mParent->GetWorldRotTrans();
	}
}

Matrix4 Actor::GetWorldRotTrans() const
{
	Matrix4 rotationMatrix = Matrix4::CreateRotationZ(mRotation);
	Matrix4 rotationXMatrix = Matrix4::CreateRotationX(mRotationX);
	Matrix4 quaternionMatrix = Matrix4::CreateFromQuaternion(mQuat);
	Matrix4 translationMatrix = Matrix4::CreateTranslation(mPosition);

	Matrix4 worldMatrix = rotationMatrix * rotationXMatrix * quaternionMatrix *
					  translationMatrix;

	if (mParent != nullptr)
		worldMatrix *= mParent->GetWorldRotTrans();

	return worldMatrix;
}

void Actor::AddChild(Actor* child)
{
	mChildren.emplace_back(child);
}

void Actor::RemoveChild(Actor* child)
{
	auto iter = std::find(mChildren.begin(), mChildren.end(), child);
	if (iter != mChildren.end())
	{
		mChildren.erase(iter);
	}
}