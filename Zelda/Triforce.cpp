//
//  Triforce.cpp
//  Game-mac
//
//  Created by Sky X on 9/30/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Triforce.hpp"
#include "Game.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "Collectible.hpp"

Triforce::Triforce(Game* game)
: Actor(game)
{
    std::vector<SDL_Texture*> tri = {
        game->GetTexture("Assets/Triforce0.png"),
        game->GetTexture("Assets/Triforce1.png")
    };
    AnimatedSprite* as = new AnimatedSprite(this);
    as->AddAnimation("tri", tri);
    as->SetAnimation("tri");
    CollisionComponent* cc = new CollisionComponent(this);
    cc->SetSize(20, 20);
    Collectible* c = new Collectible(this);
    c->SetCollect([this](){
        Mix_HaltChannel(mGame->GetBGM());
        Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/Triforce.ogg"), 0);
        GetGame()->GetPlayer()->SetState(ActorState::Paused);
    });
}
