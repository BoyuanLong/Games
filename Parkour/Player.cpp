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
#include "Arrow.hpp"
#include "Checkpoint.hpp"
#include "Math.h"

Player::Player(Game* game)
: Actor(game)
{
    new PlayerMove(this);
    new CameraComponent(this);
    CollisionComponent* cc = new CollisionComponent(this);
    cc->SetSize(PlayerSize);
}

void Player::Die(){
    //Set Position back to last respawn point
    SetPosition(respawn);
    
    //Reset Rotation to facing the next checkpoint
    auto cp = mGame->FrontCheck();
    mRotation = (cp == nullptr) ?
            0.0f : CalculateAngle(Vector3::UnitX, cp->GetPosition() - GetPosition());
    
    //Reset Camera Pitch to 0.0f
    GetComponent<CameraComponent>()->SetPitchAngle(0.0f);
}
