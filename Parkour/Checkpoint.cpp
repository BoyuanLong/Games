//
//  Checkpoint.cpp
//  Game-mac
//
//  Created by Sky X on 11/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Checkpoint.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"

#include "utils.hpp"
Checkpoint::Checkpoint(Game* game)
: Actor(game)
, active(false)
, mLevelString()
{
    Set_Mesh("Assets/Checkpoint.gpmesh");
    CollisionComponent* cc = new CollisionComponent(this);
    cc->SetSize(CheckpointSize);
}


void Checkpoint::OnUpdate(float deltaTime){
    //If the this checkpoint is the next, set texture to blue
    if(active){
        GetComponent<MeshComponent>()->SetTextureIndex(0);
    }
    else{
        GetComponent<MeshComponent>()->SetTextureIndex(1);
    }
}
