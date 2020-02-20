//
//  PlayerMove.cpp
//  Game-mac
//
//  Created by Sky X on 10/20/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "PlayerMove.hpp"
#include "Actor.h"
#include "SDL/SDL.h"
#include "Renderer.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "CameraComponent.hpp"
#include "Player.hpp"
#include "Checkpoint.hpp"

#include "utils.hpp"

PlayerMove::PlayerMove(Actor* owner)
: MoveComponent(owner)
, prevSpace(false)
, mMass(1.0f)
, mGravity(0.0f, 0.0f, -980.0f)
, mJumpForce(0.0f, 0.0f, 35000.0f)
{
    ChangeState(Falling);
    mRunningSFX = Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Running.wav"), -1);
    Mix_Pause(mRunningSFX);
}

PlayerMove::~PlayerMove(){
    HaltChannel(mRunningSFX);
}

void PlayerMove::Update(float deltaTime){
    if(mOwner->GetPosition().z < -750.0f){
        ((Player*)mOwner)->Die();
        mVelocity = mPendingForces = Vector3::Zero;
        ChangeState(Falling);
    }
    
    if((mCurrentState == OnGround && mVelocity.Length() > 50.0f) ||
       mCurrentState == WallRun || mCurrentState == WallClimb)
    {
        Mix_Resume(mRunningSFX);
    }
    else{
        Mix_Pause(mRunningSFX);
    }
    
    auto cc = mOwner->GetComponent<CollisionComponent>();
    
    if(mOwner->GetGame()->FrontCheck() != nullptr && cc->Intersect(mOwner->GetGame()->FrontCheck()->
                     GetComponent<CollisionComponent>())){
        Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Checkpoint.wav"), 0);
        auto cp = mOwner->GetGame()->RemoveCheck();
        ((Player*)mOwner)->SetSpawn(cp->GetPosition());
        mOwner->GetGame()->SetString(cp->GetString());
        cp->SetState(ActorState::Destroy);
    }
    
    switch (mCurrentState) {
        case Falling:
            UpdateFalling(deltaTime);
            break;
        case Jump:
            UpdateJump(deltaTime);
            break;
        case OnGround:
            UpdateOnGround(deltaTime);
            break;
        case WallClimb:
            UpdateWallClimb(deltaTime);
            break;
        case WallRun:
            UpdateWallRun(deltaTime);
            break;
        default:
            break;
    }
    
}

void PlayerMove::ProcessInput(const Uint8* keyState){
    if(keyState[SDL_SCANCODE_W]){
        AddForce(mOwner->GetForward() * 700.0f);
    }
    if(keyState[SDL_SCANCODE_S]){
        AddForce(mOwner->GetForward() * -700.0f);
    }

    if(keyState[SDL_SCANCODE_A]){
        AddForce(mOwner->GetRight() * -700.0f);
    }
    if(keyState[SDL_SCANCODE_D]){
        AddForce(mOwner->GetRight() * 700.0f);
    }

    
    if(!prevSpace && keyState[SDL_SCANCODE_SPACE] && mCurrentState == OnGround){
        AddForce(mJumpForce);
        Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Jump.wav"), 0);
        ChangeState(Jump);
    }
    
    int x, y;
    SDL_GetRelativeMouseState(&x, &y);
    float angular = (float)x / 500.0f * Math::Pi * 10.0f;
    SetAngularSpeed(angular);
    float angularY = (float)y / 500.0f * Math::Pi * 10.0f;
    
    mOwner->GetComponent<CameraComponent>()->SetPitchSpeed(angularY);
}

void PlayerMove::UpdateOnGround(float deltaTime){
    PhysicsUpdate(deltaTime);
    auto blocks = mOwner->GetGame()->GetBlocks();
    auto cc = mOwner->GetComponent<CollisionComponent>();
    bool onGround = false;
    for(auto b : blocks){
        Vector3 offset;
        CollSide side = FixCollision(cc, b->GetComponent<CollisionComponent>(), offset);
        if(side == CollSide::Top){
            onGround = true;
        }
        else if(side == CollSide::SideX1 ||
                side == CollSide::SideX2 ||
                side == CollSide::SideY2 ||
                side == CollSide::SideY1){
            if(CanWallClimb(side, offset)){
                mWallClimbTimer = 0.0f;
                ChangeState(WallClimb);
                break;
            }
        }
    }
    if(!onGround){
        ChangeState(Falling);
    }
}

void PlayerMove::UpdateJump(float deltaTime){
    AddForce(mGravity);
    PhysicsUpdate(deltaTime);
    
    auto blocks = mOwner->GetGame()->GetBlocks();
    auto cc = mOwner->GetComponent<CollisionComponent>();
    for(auto b : blocks){
        Vector3 offset;
        CollSide side = FixCollision(cc, b->GetComponent<CollisionComponent>(), offset);
        if(side == CollSide::Bottom){
            mVelocity.z = 0.0f;
        }
        else if(side == CollSide::SideX1 ||
                side == CollSide::SideX2 ||
                side == CollSide::SideY2 ||
                side == CollSide::SideY1){
            if(CanWallClimb(side, offset)){
                SDL_Log("climbing");
                mWallClimbTimer = 0.0f;
                ChangeState(WallClimb);
                break;
            }
            else if(CanWallRun(side, offset)){
                SDL_Log("running");
                ChangeState(WallRun);
                CameraComponent* camera = mOwner->GetComponent<CameraComponent>();
                camera->ChangeTarget(offset);
                mWallRunTimer = 0.0f;
                ChangeState(WallRun);
                break;
            }
        }
    }
    if(mVelocity.z <= 0.0f){
        ChangeState(Falling);
    }
}

void PlayerMove::UpdateFalling(float deltaTime){
    AddForce(mGravity);
    PhysicsUpdate(deltaTime);
    
    auto blocks = mOwner->GetGame()->GetBlocks();
    auto cc = mOwner->GetComponent<CollisionComponent>();
    for(auto b : blocks){
        Vector3 offset;
        if(FixCollision(cc, b->GetComponent<CollisionComponent>(), offset) == CollSide::Top){
            mVelocity.z = 0.0f;
            Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Land.wav"), 0);
            ChangeState(OnGround);
        }
    }
}

CollSide PlayerMove::FixCollision(CollisionComponent *self, CollisionComponent *block, Vector3& off){
    Vector3 offset;
    CollSide side = self->GetMinOverlap(block, offset);
    off = offset;
    if(side != CollSide::None){
        mOwner->SetPosition(mOwner->GetPosition() + offset);
    }
    if(side == CollSide::SideX1){
        AddForce(Vector3(-700.0f, 0.0f, 0.0f));
    }
    else if(side == CollSide::SideX2){
        AddForce(Vector3(700.0f, 0.0f, 0.0f));
    }
    else if(side == CollSide::SideY2){
        AddForce(Vector3(0.0f, 700.0f, 0.0f));
    }
    else if(side == CollSide::SideY1){
        AddForce(Vector3(0.0f, -700.0f, 0.0f));
    }
    
    return side;
}

void PlayerMove::AddForce(const Vector3 &force){
    mPendingForces += force;
}

void PlayerMove::PhysicsUpdate(float deltaTime){
    mAcceleration = mPendingForces * (1.0f / mMass);
    mVelocity += mAcceleration * deltaTime;
    FixXYVelocity();
    mOwner->SetPosition(mOwner->GetPosition() + mVelocity * deltaTime);
    mOwner->SetRotation(GetAngularSpeed() * deltaTime + mOwner->GetRotation());
    
    mPendingForces = Vector3::Zero;
}

void PlayerMove::FixXYVelocity(){
    Vector2 xyVelocity = Vector2(mVelocity.x, mVelocity.y);
    if(xyVelocity.Length() >= MaxSpeed){
        xyVelocity.Normalize();
        xyVelocity = xyVelocity * MaxSpeed;
    }
    if(mCurrentState == OnGround || mCurrentState == WallClimb){
        if(Math::NearZero(mAcceleration.x) || mAcceleration.x * xyVelocity.x < 0){
            xyVelocity.x *= 0.9f;
        }
        if(Math::NearZero(mAcceleration.y || mAcceleration.y * xyVelocity.y < 0)){
            xyVelocity.y *= 0.9f;
        }
    }
    
    mVelocity.x = xyVelocity.x;
    mVelocity.y = xyVelocity.y;
}


bool PlayerMove::CanWallClimb(CollSide side, Vector3& offset){
    const Vector3& forward = mOwner->GetForward();
    return Vector3::Dot(forward, Vector3::Normalize(offset)) <= -0.5f &&
       Vector3::Dot(mVelocity, offset) <= 0 &&
            mVelocity.Length() >= 350.0f;
}

void PlayerMove::UpdateWallClimb(float deltaTime){
    AddForce(mGravity);
    mWallClimbTimer += deltaTime;
    if(mWallClimbTimer < 0.4f){
        AddForce(mWallClimb);
    }
    PhysicsUpdate(deltaTime);
    
    auto blocks = mOwner->GetGame()->GetBlocks();
    auto cc = mOwner->GetComponent<CollisionComponent>();
    bool ifCollideSide = false;
    for(auto b : blocks){
        Vector3 offset;
        CollSide side = FixCollision(cc, b->GetComponent<CollisionComponent>(), offset);
        if(side == CollSide::SideX1 ||
                side == CollSide::SideX2 ||
                side == CollSide::SideY2 ||
                side == CollSide::SideY1)
        {
            ifCollideSide = true;
        }
    }
    if(!ifCollideSide || mVelocity.z <= 0.0f){
        mVelocity.z = 0.0f;
        ChangeState(Falling);
    }
}

void PlayerMove::UpdateWallRun(float deltaTime){
    AddForce(mGravity);
    mWallRunTimer += deltaTime;
    if(mWallRunTimer < 0.4f){
        AddForce(mWallRunForce);
    }
    PhysicsUpdate(deltaTime);
    
    auto blocks = mOwner->GetGame()->GetBlocks();
    auto cc = mOwner->GetComponent<CollisionComponent>();
    for(auto b : blocks){
        Vector3 offset;
        FixCollision(cc, b->GetComponent<CollisionComponent>(), offset);
    }
    
    if(mVelocity.z <= 0.0f){
        auto camera = mOwner->GetComponent<CameraComponent>();
        camera->ChangeTarget(Vector3::UnitZ);
        mVelocity.z = 0.0f;
        ChangeState(Falling);
    }
}

bool PlayerMove::CanWallRun(CollSide side, Vector3 &offset){
    const Vector3& forward = mOwner->GetForward();
    return Vector3::Dot(forward, Vector3::Normalize(offset)) <= 0.5f &&
       Vector3::Dot(mVelocity, forward) >= 0.5f &&
            mVelocity.Length() >= 350.0f;
}
