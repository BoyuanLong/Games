//
//  Block.cpp
//  Game-mac
//
//  Created by Sky X on 11/6/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Block.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"


Block::Block(Game* game,Actor* parent, bool mirror)
: Actor(game, parent)
, isMirror(mirror)
, isRotate(false)
{
    Set_Mesh("Assets/Cube.gpmesh");
    
    CollisionComponent* cc = new CollisionComponent(this);
    cc->SetSize(BlockSize);
    mScale = BlockScale;
    
    game->AddBlock(this);
}

Block::~Block(){
    mGame->RemoveBlock(this);
}

void Block::OnUpdate(float deltaTime){
    if(isRotate){
        SetRotation(Math::Pi / 4 * deltaTime + GetRotation());
    }
}
