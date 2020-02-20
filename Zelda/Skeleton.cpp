//
//  Skeleton.cpp
//  Game-mac
//
//  Created by Sky X on 9/29/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Skeleton.hpp"
#include "Game.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "EnemyComponent.hpp"
#include "Random.h"
#include "FireBall.hpp"

Skeleton::Skeleton(Game* game)
:Actor(game)
, ShootTimer(Random::GetFloatRange(1.0f, 3.0f))
{
    AnimatedSprite* as = new AnimatedSprite(this);
    std::vector<SDL_Texture*> ske = {
        mGame->GetTexture("Assets/Skeleton0.png"),
        mGame->GetTexture("Assets/Skeleton1.png")
    };
    as->AddAnimation("ske", ske);
    as->SetAnimation("ske");
    
    CollisionComponent* cc = new CollisionComponent(this);
    cc->SetSize(25, 25);
    new EnemyComponent(this);
}

void Skeleton::OnUpdate(float deltaTime){
    ShootTimer -= deltaTime;
    if(ShootTimer <= 0){
        FireBall* fb = new FireBall(mGame);
        Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/Fireball.wav"), 0);
        Vector2 dir = mGame->GetPlayer()->GetPosition() - GetPosition();
        fb->SetRotation(atan2(-dir.y, dir.x));
        fb->SetPosition(GetPosition());
        ShootTimer = 4;
    }
}
