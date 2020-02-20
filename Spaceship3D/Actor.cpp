#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>

Actor::Actor(Game* game)
	:mGame(game)
	,mState(ActorState::Active)
	,mPosition(Vector3::Zero)
	,mScale(1.0f)
	,mRotation(0.0f)
{
    game->AddActor(this);
}

Actor::~Actor()
{
    mGame->RemoveActor(this);
    for(Component* c : mComponents){
        delete c;
    }
    mComponents.clear();
}

void Actor::Update(float deltaTime)
{
    if(mState == ActorState::Active){
        for(Component* c : mComponents){
            c->Update(deltaTime);
        }
        OnUpdate(deltaTime);
    }
    mWorldTransform = Matrix4::CreateScale(mScale) * Matrix4::CreateRotationZ(mRotation) * Matrix4::CreateTranslation(mPosition);
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState)
{
    if(mState == ActorState::Active){
        for(Component* c : mComponents){
            c->ProcessInput(keyState);
        }
        OnProcessInput(keyState);
    }
}

void Actor::OnProcessInput(const Uint8* keyState)
{
}

void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}

