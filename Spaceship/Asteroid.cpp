//
//  Asteroid.cpp
//  Game-mac
//
//  Created by Sky X on 9/7/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Asteroid.hpp"
#include "Actor.h"
#include "Component.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Random.h"
#include "Game.h"




Asteroid::Asteroid(Game* game)
: Actor(game)
, _sc(new SpriteComponent(this))
, _mc(new MoveComponent(this))
{
    _sc->SetTexture(mGame->GetTexture("Assets/Asteroid.png"));
    _mc->SetForwardSpeed(SPEED);
    
    //Set Random Position and Rotation
    mRotation = Random::GetFloatRange(0.0f, Math::TwoPi);
    mPosition = Random::GetVector(Vector2(0, 0), Vector2(WINW, WINH));
    //Add to asteroid list
    game->AddAsteroid(this);
}

void Asteroid::OnUpdate(float deltaTime){
    if(mPosition.x < LOWER_MARGIN){
        mPosition.x = X_UPPER_MARGIN;
    }
    if(mPosition.x > X_UPPER_MARGIN){
        mPosition.x = LOWER_MARGIN;
    }
    if(mPosition.y < LOWER_MARGIN){
        mPosition.y = Y_UPPER_MARGIN;
    }
    if(mPosition.y > Y_UPPER_MARGIN){
        mPosition.y = -LOWER_MARGIN;
    }
}

Asteroid::~Asteroid(){
    mGame->RemoveAsteroid(this);
}
