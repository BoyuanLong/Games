//
//  FireBall.cpp
//  Game-mac
//
//  Created by Sky X on 9/29/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "FireBall.hpp"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"

FireBall::FireBall(Game* game)
: Actor(game)
, deathTimer(2.0f)
{
    SpriteComponent* sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/FireballRed.png"));
    MoveComponent* mc = new MoveComponent(this);
    mc->SetForwardSpeed(200.0f);
    CollisionComponent* cc = new CollisionComponent(this);
    cc->SetSize(16, 16);
    
}

FireBall::FireBall(Game* game, std::string color)
: Actor(game)
, deathTimer(2.0f)
{
    SpriteComponent* sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/FireballGreen.png"));
    MoveComponent* mc = new MoveComponent(this);
    mc->SetForwardSpeed(200.0f);
    CollisionComponent* cc = new CollisionComponent(this);
    cc->SetSize(16, 16);
    
}

void FireBall::OnUpdate(float deltaTime){
    deathTimer -= deltaTime;
    if(deathTimer <= 0){
        SetState(ActorState::Destroy);
    }
    if(GetComponent<CollisionComponent>()->Intersect(mGame->GetPlayer()->GetComponent<CollisionComponent>())){
        mGame->GetPlayer()->TakeDamge(1);
        SetState(ActorState::Destroy);
    }
}
