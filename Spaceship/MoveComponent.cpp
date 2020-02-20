#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner)
:Component(owner, 50)
,mAngularSpeed(0.0f)
,mForwardSpeed(0.0f)
{
	
}

void MoveComponent::Update(float deltaTime)
{
    //Update Rotation
    this->mOwner->SetRotation(mAngularSpeed * deltaTime + mOwner->GetRotation());
    //Update Position
    this->mOwner->SetPosition(mOwner->GetForward() * mForwardSpeed * deltaTime + mOwner->GetPosition());
}
