//
//  Goomba.cpp
//  Game-mac
//
//  Created by Sky X on 9/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Goomba.hpp"
#include "Game.h"
#include "CollisionComponent.h"
#include "SpriteComponent.h"
#include "GoombaMove.hpp"
#include "AnimatedSprite.h"

Goomba::Goomba(Game* game)
    : Actor(game)
    , _sc(new AnimatedSprite(this))
    , _cc(new CollisionComponent(this))
    , _gm(new GoombaMove(this))
    , isStomped(false)
    , deathTimer(GOOMBA::DeathTimer)
{
    //Set Walk Animation
    std::vector<SDL_Texture*> walkAnim{
        GetGame()->GetTexture("Assets/Goomba/Walk0.png"),
        GetGame()->GetTexture("Assets/Goomba/Walk1.png")
    };
    _sc->AddAnimation("walk", walkAnim);
    _sc->SetAnimation("walk");
    //Set Death Animation
    std::vector<SDL_Texture*> deadAnim{ GetGame()->GetTexture("Assets/Goomba/Dead.png")};
    _sc->AddAnimation("dead", deadAnim);
    _cc->SetSize(GOOMBA::GoombaSizeX, GOOMBA::GoombaSizeY);
    game->AddGoomba(this);
}

Goomba::~Goomba(){
    mGame->RemoveGoomba(this);
}

void Goomba::OnUpdate(float deltaTime){
    if(isStomped){
        _sc->SetAnimation("dead");
        deathTimer -= deltaTime;
    }
    if(deathTimer <= 0){
        SetState(ActorState::Destroy);
    }
}
