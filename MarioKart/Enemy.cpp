//
//  Enemy.cpp
//  Game-mac
//
//  Created by Sky X on 10/27/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Enemy.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "EnemyMove.hpp"

Enemy::Enemy(Game* game)
: Actor(game)
{
    mScale = 0.75f;
    MeshComponent* mc = new MeshComponent(this);
    mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Kart.gpmesh"));
    mc->SetTextureIndex(6);
    new EnemyMove(this);
    
}
