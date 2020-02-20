//
//  Block.cpp
//  Game-mac
//
//  Created by Sky X on 9/12/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Block.hpp"
#include "Game.h"
#include "CollisionComponent.h"

Block::Block(Game* game)
    : Actor(game)
    , _cc(new CollisionComponent(this))
{
    _cc->SetSize(BLOCK::BlockSizeX, BLOCK::BlockSizeY);
    mGame->AddBlock(this);
}

Block::~Block(){
    mGame->DeleteBlock(this);
}
