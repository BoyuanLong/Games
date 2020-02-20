//
//  Ship.cpp
//  Game-mac
//
//  Created by Sky X on 9/6/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Ship.hpp"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Laser.hpp"



Ship::Ship(Game* game)
    : Actor(game)
    , laserCD(0)
    , _sc(new SpriteComponent(this))
    ,_mc(new MoveComponent(this))
{
    _sc->SetTexture(mGame->GetTexture("Assets/Ship.png"));
}

void Ship::OnProcessInput(const Uint8* keyState){
    //Movement
    if(keyState[SDL_SCANCODE_UP] && !keyState[SDL_SCANCODE_DOWN]){
        _mc->SetForwardSpeed(Ship_SPEED);
        _sc->SetTexture(mGame->GetTexture("Assets/ShipThrust.png"));
    }
    else if(keyState[SDL_SCANCODE_DOWN] && !keyState[SDL_SCANCODE_UP]){
        _mc->SetForwardSpeed(-Ship_SPEED);
        _sc->SetTexture(mGame->GetTexture("Assets/ShipThrust.png"));
    }
    else{
        _mc->SetForwardSpeed(0);
        _sc->SetTexture(mGame->GetTexture("Assets/Ship.png"));
    }
    
    //Rotation
    if(keyState[SDL_SCANCODE_LEFT] && !keyState[SDL_SCANCODE_RIGHT]){
        _mc->SetAngularSpeed(static_cast<float>(ANGULAR_SPEED));
    }
    else if(keyState[SDL_SCANCODE_RIGHT] && !keyState[SDL_SCANCODE_LEFT]){
        _mc->SetAngularSpeed(static_cast<float>(-ANGULAR_SPEED));
    }
    else{
        _mc->SetAngularSpeed(0);
    }
    
    if(keyState[SDL_SCANCODE_SPACE] && laserCD > laserCoolDown){
        Laser* l = new Laser(mGame);
        l->SetPosition(mPosition);
        l->SetRotation(mRotation);
        laserCD = 0;
    }
}


void Ship::OnUpdate(float deltaTime){
    laserCD += deltaTime;
}
