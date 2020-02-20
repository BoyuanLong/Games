//
//  Door.cpp
//  Game-mac
//
//  Created by Sky X on 9/27/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Door.hpp"
#include "Game.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"

Door::Door(Game* game)
: Actor(game)

{
    new SpriteComponent(this);
    new CollisionComponent(this);
}

void Door::SetUp(std::string state, std::string dir, std::string dist){
    if(state == "Open"){
        mState = DoorState::Open;
    }
    else if(state == "Closed"){
        mState = DoorState::Closed;
    }
    else if(state == "Locked"){
        mState = DoorState::Locked;
    }
    
    if(dir == "Up"){
        mDirection = DoorDirection::Up;
    }
    else if(dir == "Down"){
        mDirection = DoorDirection::Down;
    }
    else if(dir == "Left"){
        mDirection = DoorDirection::Left;
    }
    else if(dir == "Right"){
        mDirection = DoorDirection::Right;
    }
    distRoom = dist;
}

void Door::UpdateTexture(){
    CollisionComponent* _cc = GetComponent<CollisionComponent>();
    
    std::string name = "Assets/Door/";
    switch (mDirection) {
        case DoorDirection::Up:
            name += "Up";
            _cc->SetSize(30, 64);
            break;
        case DoorDirection::Down:
            name += "Down";
            _cc->SetSize(30, 64);
            break;
        case DoorDirection::Left:
            name += "Left";
            _cc->SetSize(64, 30);
            break;
        case DoorDirection::Right:
            name += "Right";
            _cc->SetSize(64, 30);
            break;
    }
    switch (mState) {
        case DoorState::Open:
            name += "Open";
            break;
        case DoorState::Closed:
            name += "Closed";
            break;
        case DoorState::Locked:
            name += "Locked";
            break;
    }
    name += ".png";
    SpriteComponent* _sc = GetComponent<SpriteComponent>();
    _sc->SetTexture(mGame->GetTexture(name));
}
