//
//  Collider.cpp
//  Game-mac
//
//  Created by Sky X on 9/26/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Collider.hpp"
#include "Game.h"
#include "CollisionComponent.h"

Collider::Collider(Game* game)
: Actor(game)
, _cc(new CollisionComponent(this)){
    
}

void Collider::SetDimention(float width, float height){
    mWidth = width;
    mHeight = height;
    _cc->SetSize(mWidth, mHeight);
}
