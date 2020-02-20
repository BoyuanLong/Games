//
//  SideBlock.cpp
//  Game-mac
//
//  Created by Sky X on 10/20/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "SideBlock.hpp"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.hpp"

SideBlock::SideBlock(Game* game, int count)
: Actor(game)
{
    mScale = 500;
    MeshComponent* mc = new MeshComponent(this);
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
    mc->SetTextureIndex(textureArr[count % 4]);
}

void SideBlock::OnUpdate(float deltaTime){
    if(GetPosition().x < mGame->GetPlayer()->GetPosition().x - 2000.0f){
        SetState(ActorState::Destroy);
    }
}
