//
//  Bat.cpp
//  Game-mac
//
//  Created by Sky X on 9/29/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Bat.hpp"
#include "Game.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "EnemyComponent.hpp"
#include "Random.h"
#include "Collider.hpp"
#include "MoveComponent.h"

Bat::Bat(Game* game)
: Actor(game)
, timeremain(0.0f)
, direction(Vector2(0, 0))
{
    AnimatedSprite* sc = new AnimatedSprite(this);
    std::vector<SDL_Texture*> fly = {
        mGame->GetTexture("Assets/Bat0.png"),
        mGame->GetTexture("Assets/Bat1.png")
    };
    sc->AddAnimation("fly", fly);
    sc->SetAnimation("fly");
    CollisionComponent* cc = new CollisionComponent(this);
    cc->SetSize(25, 25);
    new EnemyComponent(this);
    
}

void Bat::OnUpdate(float deltaTime){
    
    if(timeremain <= 0){
        direction = Vector2(Random::GetFloatRange(-1, 1), Random::GetFloatRange(-1, 1));
        direction.Normalize();
        timeremain = 3.0f;
    }
    
        SetPosition(GetPosition() + 50 * direction * deltaTime);
        
        timeremain -= deltaTime;
        Vector2 offset;
        std::vector<Collider*> colliders = GetGame()->GetColliders();
        for(Collider* c : colliders){
            CollSide side = GetComponent<CollisionComponent>()->GetMinOverlap(c->GetComponent<CollisionComponent>(), offset);
            if(side != CollSide::None){
                SetPosition(GetPosition() + offset);
                //break;
            }
        }
}
