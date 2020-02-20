//
//  Block.cpp
//  Game-mac
//
//  Created by Sky X on 10/20/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Block.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "Player.hpp"


Block::Block(Game* game, bool isExplosing)
: Actor(game)
, explosing(isExplosing){
    mScale = 25.0f;
    MeshComponent* mc = new MeshComponent(this);
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
    if(explosing){
        mc->SetTextureIndex(4);
    }
    else{
        mc->SetTextureIndex(2);
    }
    
    CollisionComponent* cc = new CollisionComponent(this);
    cc->SetSize(1, 1, 1);
    game->AddBlock(this);
}

Block::~Block(){
    mGame->DeleteBlocK(this);
}

void Block::OnUpdate(float deltaTime){
    if(GetPosition().x < mGame->GetPlayer()->GetPosition().x - 2000.0f){
        SetState(ActorState::Destroy);
    }
}

void Block::Explode(){
    SetState(ActorState::Destroy);
    if(explosing){
        auto blocks = mGame->GetBlock();
        for(auto b : blocks){
            if(b->GetState() == ActorState::Active && (b->GetPosition() - GetPosition()).Length() <= 50){
                b->Explode();
            }
        }
        
    }
}
