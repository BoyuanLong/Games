//
//  Collectible.cpp
//  Game-mac
//
//  Created by Sky X on 9/29/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Collectible.hpp"
#include "Player.hpp"
#include "CollisionComponent.h"
#include "Game.h"

Collectible::Collectible(Actor* onwer)
: Component(onwer){
    
}

void Collectible::Update(float deltaTime){
    Player* p = mOwner->GetGame()->GetPlayer();
    if(mOwner->GetComponent<CollisionComponent>()->Intersect(p->GetCC())){
        mOwner->SetState(ActorState::Destroy);
        Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Key.wav"), 0);
        if(mOnCollect){
            mOnCollect();
        }
    }
}
