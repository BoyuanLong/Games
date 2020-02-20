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
{
    std::vector<SDL_Texture*> idleAnim = {mGame->GetTexture("Assets/Mario/Idle.png")},
    deadAnim = {mGame->GetTexture("Assets/Mario/Dead.png")},
    jumpLeft = {mGame->GetTexture("Assets/Mario/jumpLeft.png")},
    jumpRight = {mGame->GetTexture("Assets/Mario/jumpRight.png")},
    runLeft = {mGame->GetTexture("Assets/Mario/RunLeft0.png"), mGame->GetTexture("Assets/Mario/RunLeft1.png"), mGame->GetTexture("Assets/Mario/RunLeft2.png")},
    runRight = {mGame->GetTexture("Assets/Mario/RunRight0.png"), mGame->GetTexture("Assets/Mario/RunRight1.png"),
        mGame->GetTexture("Assets/Mario/RunRight2.png")
    };
    
    _sc->AddAnimation("idle", idleAnim);
    _sc->AddAnimation("dead", deadAnim);
    _sc->AddAnimation("jumpLeft", jumpLeft);
    _sc->AddAnimation("jumpRight", jumpRight);
    _sc->AddAnimation("runLeft", runLeft);
    _sc->AddAnimation("runRight", runRight);
    _sc->SetAnimation("idle");
    _cc->SetSize(PLAYER::MarioSizeX, PLAYER::MarioSizeY);
}
