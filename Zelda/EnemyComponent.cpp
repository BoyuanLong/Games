//
//  EnemyComponent.cpp
//  Game-mac
//
//  Created by Sky X on 9/29/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "EnemyComponent.hpp"
#include "Actor.h"
#include "Game.h"

EnemyComponent::EnemyComponent(Actor* owner)
: Component(owner)
, hitPoint(1)
, damage(1)
, invulnerableTimer(0)
, isBoss(false)
{
    mOwner->GetGame()->AddEnemy(mOwner, mOwner->GetGame()->GetCurrentRoom());
}

EnemyComponent::~EnemyComponent(){
    mOwner->GetGame()->RemoveEnemy(mOwner, mOwner->GetGame()->GetCurrentRoom());
}

void EnemyComponent::TakeDamage(int amount){
    if(invulnerableTimer <= 0){
        hitPoint -= amount;
        invulnerableTimer = 1;
        if(isBoss){
            Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/BossHit.wav"), 0);
        }
    }
    
    if(hitPoint <= 0){
        if(mOnDeath){
            mOnDeath();
        }
        if(isBoss){
            Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/BossDie.wav"), 0);
            std::vector<Door*> doors = mOwner->GetGame()->GetDoors(mOwner->GetGame()->GetCurrentRoom());
            for(Door* d : doors){
                if(d->getState() == DoorState::Closed){
                    d->SetState(DoorState::Open);
                    d->UpdateTexture();
                    Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/DoorOpen.wav"), 0);
                }
            }
        }
        Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/EnemyDie.wav"), 0);
        mOwner->SetState(ActorState::Destroy);
    }
    else{
        if(mOnTakeDamge){
            mOnTakeDamge();
        }
    }
}

void EnemyComponent::Update(float deltaTime){
    invulnerableTimer -= deltaTime;
}
