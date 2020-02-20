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
#include "Bullet.hpp"
#include "Game.h"
#include "CollisionComponent.h"
#include "Block.hpp"

PlayerMove::PlayerMove(Actor* owner)
: MoveComponent(owner)
, mVelocity(400, 0, 0)
, lastFrameSpace(false)
, mutiplyTimer(0.0f)
, vMultiplier(1.0f)
{
    
}

void PlayerMove::Update(float deltaTime){
    mutiplyTimer += deltaTime;
    if(mutiplyTimer >= 10.0f){
        mutiplyTimer = 0.0f;
        vMultiplier += 0.15f;
    }
    Vector3 pos = mOwner->GetPosition() + mVelocity * deltaTime * vMultiplier;
    if(pos.y > PLAYER::YBound){
        pos.y = PLAYER::YBound;
    }
    if(pos.y < -PLAYER::YBound){
        pos.y = -PLAYER::YBound;
    }
    if(pos.z > PLAYER::ZBound){
        pos.z = PLAYER::ZBound;
    }
    if(pos.z < -PLAYER::ZBound){
        pos.z = -PLAYER::ZBound;
    }
    mOwner->SetPosition(pos);
    
    float HDist = 300.0f;
    float VDist = 100.0f;
    float TargetDist = 20.0f;
    
    Matrix4 lookat = Matrix4::CreateLookAt(pos + Vector3(-HDist, 0, VDist), pos + Vector3(TargetDist, 0, 0), Vector3::UnitZ);
    mOwner->GetGame()->GetRenderer()->SetViewMatrix(lookat);
    
    auto blocks = mOwner->GetGame()->GetBlock();
    auto cc = mOwner->GetComponent<CollisionComponent>();
    for(auto b : blocks){
        if(cc->Intersect(b->GetComponent<CollisionComponent>())){
            mOwner->SetState(ActorState::Paused);
            Mix_PlayChannel(1, mOwner->GetGame()->GetSound("Assets/Sounds/ShipDie.wav"), 0);
        }
    }
}

void PlayerMove::ProcessInput(const Uint8* keyState){
    mVelocity = Vector3(400 * vMultiplier, 0, 0);
    if(keyState[SDL_SCANCODE_W]){
        mVelocity.z += 300 * vMultiplier;
    }
    if(keyState[SDL_SCANCODE_S]){
        mVelocity.z -= 300 * vMultiplier;
    }
    if(keyState[SDL_SCANCODE_A]){
        mVelocity.y -= 300 * vMultiplier;
    }
    if(keyState[SDL_SCANCODE_D]){
        mVelocity.y += 300 * vMultiplier;
    }
    if(keyState[SDL_SCANCODE_SPACE] && !lastFrameSpace){
        Bullet* b = new Bullet(mOwner->GetGame());
        b->SetPosition(mOwner->GetPosition());
    }
    lastFrameSpace = keyState[SDL_SCANCODE_SPACE];
}
