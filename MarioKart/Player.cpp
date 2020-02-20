//
//  Player.cpp
//  Game-mac
//
//  Created by Sky X on 10/20/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Player.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "PlayerMove.hpp"
#include "CameraComponent.hpp"
#include "PlayerUI.h"


Player::Player(Game* game)
: Actor(game)
{
    mMeshComponent = new MeshComponent(this);
    mMeshComponent->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Kart.gpmesh"));
    this->SetScale(0.75f);
    
    new PlayerMove(this);
    new CameraComponent(this);
    new PlayerUI(this);
}
