//
//  Sword.cpp
//  Game-mac
//
//  Created by Sky X on 9/30/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Sword.hpp"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Game.h"

Sword::Sword(Game* game)
: Actor(game){
    SpriteComponent* sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/Sword/Up.png"));
    CollisionComponent* cc = new CollisionComponent(this);
    cc->SetSize(25, 25);
    
}
