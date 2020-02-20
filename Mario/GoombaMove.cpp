//
//  GoombaMove.cpp
//  Game-mac
//
//  Created by Sky X on 9/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "GoombaMove.hpp"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "Goomba.hpp"
#include "Game.h"
#include "Block.hpp"
#include <vector>
using std::vector;

GoombaMove::GoombaMove(Actor* owner)
    : MoveComponent(owner)
{
    SetForwardSpeed(-GOOMBA::SPEED);
}

void GoombaMove::Update(float deltaTime){
    //If Goomba got stomped, do nothing
    if(reinterpret_cast<Goomba*>(mOwner)->GetStomped()) return;
    
    HitGoomba(deltaTime);
    HitBlock(deltaTime);
    OnGround(deltaTime);
    
    MoveComponent::Update(deltaTime);
}

void GoombaMove::HitGoomba(float deltaTime){
    //If two goomba collide with each other, they should move towards
    //different directions
    vector<Goomba*> goombas = mOwner->GetGame()->GetGoombas();
    for(Goomba* g : goombas){
        if(g == mOwner)   continue;
        
        Vector2 offset;
        CollisionComponent* cc = mOwner->GetComponent<CollisionComponent>();
        CollSide side = cc->GetMinOverlap(g->GetComponent<CollisionComponent>(), offset);
        
        if((side == CollSide::Left) && GetForwardSpeed() < 0)
        {
            SetForwardSpeed(-GetForwardSpeed());
            g->GetGM()->SetForwardSpeed(-(g->GetGM()->GetForwardSpeed()));
            mOwner->SetPosition(mOwner->GetPosition() + offset);
            break;
        }
        if((side == CollSide::Right) && GetForwardSpeed() > 0){
            SetForwardSpeed(-GetForwardSpeed());
            g->GetGM()->SetForwardSpeed(-(g->GetGM()->GetForwardSpeed()));
            mOwner->SetPosition(mOwner->GetPosition() + offset);
            break;
        }
    }
}

void GoombaMove::HitBlock(float deltaTime){
    //If our little Goomba touches the block, it should bounce and move towards
    //a different direction
    vector<Block*> blocks = mOwner->GetGame()->GetBlocks();
    for(Block* b : blocks){
        Vector2 offset;
        CollSide side = reinterpret_cast<Goomba*>(mOwner)->GetCC()->GetMinOverlap(b->GetCC(), offset);
        if((side == CollSide::Left) && GetForwardSpeed() < 0 && abs(b->GetPosition().y - mOwner->GetPosition().y) < 20)
        {
            SetForwardSpeed(GOOMBA::SPEED);
            mOwner->SetPosition(mOwner->GetPosition() + offset);
            break;
        }
        if((side == CollSide::Right) && GetForwardSpeed() > 0 && abs(b->GetPosition().y - mOwner->GetPosition().y) < 20){
            SetForwardSpeed(-GOOMBA::SPEED);
            mOwner->SetPosition(mOwner->GetPosition() + offset);
            break;
        }
    }
}

void GoombaMove::OnGround(float deltaTime){
    bool isGround = false;
    vector<Block*> blocks = mOwner->GetGame()->GetBlocks();
    //Loop over all blocks to see if little Goomba is standing on ground
    for(Block* b : blocks){
        Vector2 offset;
        if(reinterpret_cast<Goomba*>(mOwner)->GetCC()->
           GetMinOverlap(b->GetCC(), offset) == CollSide::Bottom){
            yspeed = 0;
            mOwner->SetPosition(mOwner->GetPosition() + offset);
            isGround = true;
        }
    }
    //if(!isGround){
        yspeed += GAME::Gravity * deltaTime;
        mOwner->SetPosition(mOwner->GetPosition() + Vector2(0, yspeed * deltaTime));
    //}
}
