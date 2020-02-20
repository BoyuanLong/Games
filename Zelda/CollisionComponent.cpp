#include "CollisionComponent.h"
#include "Actor.h"

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f)
{
	
}

CollisionComponent::~CollisionComponent()
{
	
}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
	return !(GetMin().x > other->GetMax().x ||
            GetMin().y > other->GetMax().y ||
            GetMax().x < other->GetMin().x ||
            GetMax().y < other->GetMin().y);
}

Vector2 CollisionComponent::GetMin() const
{
    float x = mOwner->GetPosition().x - (mWidth * mOwner->GetScale()) / 2.0f;
    float y = mOwner->GetPosition().y - (mHeight * mOwner->GetScale()) / 2.0f;
	return Vector2(x, y);
}

Vector2 CollisionComponent::GetMax() const
{
    float x = mOwner->GetPosition().x + (mWidth * mOwner->GetScale()) / 2.0f;
    float y = mOwner->GetPosition().y + (mHeight * mOwner->GetScale()) / 2.0f;
	return Vector2(x, y);
}

const Vector2& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(
	const CollisionComponent* other, Vector2& offset)
{
    if(!Intersect(other)){
        offset = Vector2::Zero;
        return CollSide::None;
    }
    else{
        float dx1 = abs(other->GetMin().x - GetMax().x);
        float dx2 = abs(other->GetMax().x - GetMin().x);
        float dy1 = abs(other->GetMin().y - GetMax().y);
        float dy2 = abs(other->GetMax().y - GetMin().y);
        
        if(dx1 <= dx2 && dx1 <= dy1 && dx1 <= dy2){
            offset = Vector2(-dx1, 0);
            return CollSide::Right;
        }
        else if(dx2 <= dx1 && dx2 <= dy1 && dx2 <= dy2){
            offset = Vector2(dx2, 0);
            return CollSide::Left;
        }
        else if(dy1 <= dx1 && dy1 <= dx2 && dy1 <= dy2){
            offset = Vector2(0, -dy1);
            return CollSide::Bottom;
        }
        else{
            offset = Vector2(0, dy2);
            return CollSide::Top;
        }
    }
}
