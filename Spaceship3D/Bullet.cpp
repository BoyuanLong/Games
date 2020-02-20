//
//  Bullet.cpp
//  Game-mac
//
//  Created by Sky X on 10/20/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Bullet.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "MoveComponent.h"
#include "Block.hpp"
#include "PlayerMove.hpp"
#include "Player.hpp"


Bullet::Bullet(Game* game)
: Actor(game)
, lifeTime(BULLET::Clock){
    Mix_PlayChannel(-1, game->GetSound("Assets/Sounds/Shoot.wav"), 0);
    mScale = 0.5f;
    
    MeshComponent* mc  = new MeshComponent(this);
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Sphere.gpmesh"));
    
    CollisionComponent* cc = new CollisionComponent(this);
    cc->SetSize(10, 10, 10);
    MoveComponent* move = new MoveComponent(this);
    move->SetForwardSpeed(mGame->GetPlayer()->GetComponent<PlayerMove>()->GetVelocity().x + BULLET::Speed);
}

void Bullet::OnUpdate(float deltaTime){
    lifeTime -= deltaTime;
    if(lifeTime <= 0){
        SetState(ActorState::Destroy);
        return;
    }
    
    std::vector<Block*> blocks = mGame->GetBlock();
    CollisionComponent* cc = GetComponent<CollisionComponent>();
    for(auto b : blocks){
        if(cc->Intersect(b->GetComponent<CollisionComponent>())){
            if(b->isExplodable()){
                b->Explode();
                Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/BlockExplode.wav"), 0);
            }
            SetState(ActorState::Destroy);
            break;
        }
    }
}
