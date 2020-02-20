//
//  Player.cpp
//  Game-mac
//
//  Created by Sky X on 9/19/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Player.hpp"
#include "PlayerMove.hpp"
#include "SpriteComponent.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "Game.h"

Player::Player(Game* game)
    : Actor(game)
    , _sc(new AnimatedSprite(this, 200))
    , _cc(new CollisionComponent(this))
    , _pm(new PlayerMove(this))
    , keyCount(0)
    , hitPoint(10)
{
    
    //Create Texture Animation
    std::vector<SDL_Texture*>
    walkUP = {
        mGame->GetTexture("Assets/Link/WalkUp0.png"),
        mGame->GetTexture("Assets/Link/WalkUp1.png")
    },
    walkDown = {
        mGame->GetTexture("Assets/Link/WalkDown0.png"),
        mGame->GetTexture("Assets/Link/WalkDown1.png")
        
    },
    walkLeft = {
        mGame->GetTexture("Assets/Link/WalkLeft0.png"),
        mGame->GetTexture("Assets/Link/WalkLeft1.png")
    },
    walkRight = {
        mGame->GetTexture("Assets/Link/WalkRight0.png"),
        mGame->GetTexture("Assets/Link/WalkRight1.png")
    },
    attackUp = {mGame->GetTexture("Assets/Link/AttackUp.png")},
    attackDown = {mGame->GetTexture("Assets/Link/AttackDown.png")},
    attackLeft = {mGame->GetTexture("Assets/Link/AttackLeft.png")},
    attackRight = {mGame->GetTexture("Assets/Link/AttackRight.png")};
    
    //Add Animation list
    _sc->AddAnimation("walkUp", walkUP);
    _sc->AddAnimation("walkDown", walkDown);
    _sc->AddAnimation("walkLeft", walkLeft);
    _sc->AddAnimation("walkRight", walkRight);
    _sc->AddAnimation("attackUp", attackUp);
    _sc->AddAnimation("attackDown", attackDown);
    _sc->AddAnimation("attackLeft", attackLeft);
    _sc->AddAnimation("attackRight", attackRight);
    
    //Set starting animation
    _sc->SetAnimation("walkUp");
    _sc->SetIsPaused(true);
    
    //Set CollisionBox Size
    _cc->SetSize(20, 20);
}

void Player::SetAnimation(std::string name){
    if(name.compare("pause") == 0){
        _sc->SetIsPaused(true);
    }
    else if(name.size() == 0){
        return;
    }
    else{
        _sc->SetIsPaused(false);
        _sc->SetAnimation(name);
    }
}

void Player::TakeDamge(int amount){
    hitPoint -= amount;
    if(hitPoint <= 0){
        SetState(ActorState::Paused);
        Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/LinkDie.wav"), 0);
    }
    else{
        Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/LinkHit.wav"), 0);
    }
}
