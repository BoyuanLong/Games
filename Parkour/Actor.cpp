#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include <algorithm>

Actor::Actor(Game* game, Actor* parent)
	:mGame(game)
	,mState(ActorState::Active)
	,mPosition(Vector3::Zero)
	,mScale(1.0f)
	,mRotation(0.0f)
    ,mQua()
    ,mParent(parent)
{
    if(parent){
        parent->AddChild(this);
    }
    else{
        game->AddActor(this);
    }
    
}

Actor::~Actor()
{
    if(mParent){
        mParent->RemoveChild(this);
    }
    else{
        mGame->RemoveActor(this);
    }
    
    for(Component* c : mComponents){
        delete c;
    }
    mComponents.clear();
    
    while (!mChildren.empty()) {
        delete mChildren.back();
    }
}

void Actor::Update(float deltaTime)
{
    CalcWorldTransform();
    if(mState == ActorState::Active){
        for(Component* c : mComponents){
            c->Update(deltaTime);
        }
        OnUpdate(deltaTime);
    }
    CalcWorldTransform();
    for(Actor* c : mChildren){
        c->Update(deltaTime);
    }
}

void Actor::CalcWorldTransform(){
    mWorldTransform = Matrix4::CreateScale(mScale) * Matrix4::CreateRotationZ(mRotation) * Matrix4::CreateFromQuaternion(mQua) * Matrix4::CreateTranslation(mPosition);
    if(mParent){
        if(mInheritScale){
            mWorldTransform *= mParent->GetWorldTransform();
        }
        else{
            mWorldTransform *= mParent->CalcWorldRotTrans();
        }
    }
}

Matrix4 Actor::CalcWorldRotTrans(){
    Matrix4 res = Matrix4::CreateRotationZ(mRotation) * Matrix4::CreateFromQuaternion(mQua) * Matrix4::CreateTranslation(mPosition);
    if(mParent){
        res *= mParent->CalcWorldRotTrans();
    }
    return res;
}

Vector3 Actor::GetWorldPosition(){
    return mWorldTransform.GetTranslation();
}

Vector3 Actor::GetWorldForward(){
    return mWorldTransform.GetXAxis();
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

Vector3 Actor::GetQuatForwar(){
    Vector3 transform = Vector3::Transform(Vector3::UnitX, mQua);
    transform.Normalize();
    return transform;
}


void Actor::RemoveChild(Actor *child){
    auto iter = std::find(mChildren.begin(), mChildren.end(), child);
    if (iter != mChildren.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        auto iter2 = mChildren.end() - 1;
        std::iter_swap(iter, iter2);
        mChildren.pop_back();
    }
}

void Actor::Set_Mesh(const std::string &filename, bool alpha){
    MeshComponent* mc = GetComponent<MeshComponent>();
    if(mc == nullptr){
        mc = new MeshComponent(this, alpha);
    }
    mc->SetMesh(mGame->GetRenderer()->GetMesh(filename));
}
