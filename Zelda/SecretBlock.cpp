//
//  SecretBlock.cpp
//  Game-mac
//
//  Created by Sky X on 9/28/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "SecretBlock.hpp"
#include "SpriteComponent.h"
#include "Game.h"
#include "Door.hpp"

SecretBlock::SecretBlock(Game* game, std::string direction, Vector2 origin)
: Actor(game)
, mOrigin(origin)
{
    if(direction == "Left"){
        mDirection = CollSide::Right;
    }
    else if(direction == "Right"){
        mDirection = CollSide::Left;
    }
    else if(direction == "Up"){
        mDirection = CollSide::Bottom;
    }
    else if(direction == "Down"){
        mDirection = CollSide::Top;
    }
    
    SpriteComponent* _sc = new SpriteComponent(this);
    _sc->SetTexture(mGame->GetTexture("Assets/SecretBlock.png"));
}

CollSide SecretBlock::getDir(const Vector2& dir){
    float x = dir.x;
    float y = dir.y;
    if(x == 0 && y == 1){
        return CollSide::Top;
    }
    else if(x == 0 && y == -1){
        return CollSide::Bottom;
    }
    else if(x == 1 && y == 0){
        return CollSide::Left;
    }
    else if(x == -1 && y == 0){
        return CollSide::Right;
    }
    return CollSide::None;
}

void SecretBlock::Move(float deltaTime, const Vector2 &dir){
    //if(getDir(dir) != mDirection) return;
    if((GetPosition() - mOrigin).Length() < SECRETBLOCK::Range){
       SetPosition(GetPosition() + dir * SECRETBLOCK::SpeedLimit * deltaTime);
    }
    else{
        Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/Secret.wav"), 0);
        std::vector<Door*> doors = mGame->GetDoors(mGame->GetCurrentRoom());
        for(Door* d : doors){
            if(d->getState() == DoorState::Closed){
                d->SetState(DoorState::Open);
                d->UpdateTexture();
                Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/DoorOpen.wav"), 0);
            }
        }
    }
}
