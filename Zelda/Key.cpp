//
//  Key.cpp
//  Game-mac
//
//  Created by Sky X on 9/29/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Key.hpp"
#include "Game.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Collectible.hpp"

Key::Key(Game* game)
: Actor(game){
    SpriteComponent* sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/Key.png"));
    CollisionComponent* cc = new CollisionComponent(this);
    cc->SetSize(KEY::SizeX, KEY::SizeY);
    Collectible* c = new Collectible(this);
    c->SetCollect([this](){
        this->GetGame()->GetPlayer()->KeyPickUp();
    });
}
