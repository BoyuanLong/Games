//
//  PlayerMove.cpp
//  Game-mac
//
//  Created by Sky X on 9/19/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "PlayerMove.hpp"
#include "SDL/SDL.h"
#include "Actor.h"
#include "Game.h"
#include "Block.hpp"
#include "CollisionComponent.h"
#include "Player.hpp"
#include "Goomba.hpp"
#include "AnimatedSprite.h"
#include "SDL/SDL_mixer.h"

PlayerMove::PlayerMove(Actor* actor)
: MoveComponent(actor)
, mYSpeed(0.0f)
, mSpacePressed(false)
, mInAir(false)
{
    
}


void PlayerMove::ProcessInput(const Uint8* keyState){
    if(keyState[SDL_SCANCODE_LEFT]){
        SetForwardSpeed(-PLAYER::SPEED);
    }
    else if(keyState[SDL_SCANCODE_RIGHT]){
        SetForwardSpeed(PLAYER::SPEED);
    }
    else{
        SetForwardSpeed(0.0f);
    }
    
    if(!mSpacePressed && keyState[SDL_SCANCODE_SPACE]){
        if(!mInAir){
            mYSpeed = PLAYER::JumpSpeed;
            mInAir = true;
            Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Jump.wav"), 0);
        }
    }

    mSpacePressed = keyState[SDL_SCANCODE_SPACE];
}

void PlayerMove::Update(float deltaTime){
    //Update Position
    Vector2 update(GetForwardSpeed() * deltaTime, mYSpeed * deltaTime);
    mOwner->SetPosition(update + mOwner->GetPosition());
    
    Stalker();
    
    //If Mario is inAir, the Gravity will reduce Yspeed
    CollideWithBlocks(deltaTime);
    mYSpeed += GAME::MarioGravity * deltaTime;
    
    //Stomping Goombas!
    StompGoombas(deltaTime);
    //Update Camera Position
    mOwner->GetGame()->CameraManager(mOwner->GetPosition());
    
    AnimationManager();
}

void PlayerMove::Stalker(){
    Vector2 mario = mOwner->GetPosition();
    //Check if Mario goes out of scope
    if(mario.y > GAME::GoundLevel){
        mOwner->SetState(ActorState::Paused);
    }
    if(mario.x > GAME::Destination){
        Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/StageClear.wav"), 0);
        mOwner->SetState(ActorState::Paused);
        mOwner->GetGame()->PlayerWin();
    }
    
    //Disallowing Mario goes left of Camera
    Vector2 camera = mOwner->GetGame()->getCamera();
    if(mario.x < camera.x){
        mOwner->SetPosition(Vector2(camera.x, mario.y));
    }
}

bool PlayerMove::CollideWithBlocks(float deltaTime){
    std::vector<Block*> blocks = mOwner->GetGame()->GetBlocks();
    bool isCollide = false;
    for(Block* b : blocks){
        Vector2 offest;
        CollSide state = reinterpret_cast<Player*>(mOwner)->GetCC()->GetMinOverlap(b->GetCC(), offest);
        if(state == CollSide::None) continue;
        //If Mario is standing on blocks
        else if(state == CollSide::Bottom){
            if(mYSpeed > 0.0f){
                isCollide = true;
                mYSpeed = 0.0f;
                mOwner->SetPosition(mOwner->GetPosition() + offest);
                mInAir = false;
            }
            else{
                mYSpeed = 0.0f;
            }
        }
        //If Mario's head hits blocks
        else if(state == CollSide::Top && mYSpeed < 0.0f){
            mYSpeed = 0.0f;
            Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Bump.wav"), 0);
        }
        else{
            isCollide = true;
            mOwner->SetPosition(mOwner->GetPosition() + offest);
        }
    }
    return isCollide;
}

void PlayerMove::StompGoombas(float deltaTime){
    //If Mario is inAir and collide any Goomba on the bottom,
    //The goomba got stomped and killed
    //And Mario will get a miniJump with half of the speed
    std::vector<Goomba*> goombas = mOwner->GetGame()->GetGoombas();
    for(Goomba* g : goombas){
        Vector2 offset;
        CollSide state = reinterpret_cast<Player*>(mOwner)->GetCC()->
        GetMinOverlap(g->GetCC(), offset);
        if(state == CollSide::Bottom ||
           (mInAir && (state == CollSide::Left || state == CollSide::Right))){
            g->Stomped();
            mYSpeed = PLAYER::JumpSpeed / 2;
            mInAir = true;
            Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Stomp.wav"), 0);
        }
        else if(state != CollSide::None && !g->GetStomped()){
            mOwner->SetState(ActorState::Paused);
        }
    }
}

void PlayerMove::AnimationManager(){
    if(!mInAir){
        //Running Animation
        if(GetForwardSpeed() > 1){
            mOwner->GetComponent<AnimatedSprite>()->SetAnimation("runRight");
        }
        else if(GetForwardSpeed() < -1){
            mOwner->GetComponent<AnimatedSprite>()->SetAnimation("runLeft");
        }
        else{
            mOwner->GetComponent<AnimatedSprite>()->SetAnimation("idle");
        }
    }
    else{
        //Jump Animation
        if(GetForwardSpeed() > 1){
            mOwner->GetComponent<AnimatedSprite>()->SetAnimation("jumpRight");
        }
        else if(GetForwardSpeed() < -1){
            mOwner->GetComponent<AnimatedSprite>()->SetAnimation("jumpLeft");
        }
        else{
            if(mOwner->GetComponent<AnimatedSprite>()->GetAnimName() == "runRight" ||
               mOwner->GetComponent<AnimatedSprite>()->GetAnimName() == "jumpRight" ||
               mOwner->GetComponent<AnimatedSprite>()->GetAnimName() == "idle"){
                mOwner->GetComponent<AnimatedSprite>()->SetAnimation("jumpRight");
            }
            else{
                mOwner->GetComponent<AnimatedSprite>()->SetAnimation("jumpLeft");
            }
        }
    }
    //Mario Died
    if(mOwner->GetState() == ActorState::Paused && !mOwner->GetGame()->ifWin()){
        mOwner->GetComponent<AnimatedSprite>()->SetAnimation("dead");
    }
}
