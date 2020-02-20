//
//  Laser.cpp
//  Game-mac
//
//  Created by Sky X on 9/7/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Laser.hpp"
#include "Game.h"
#include "Actor.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "Asteroid.hpp"
#include "Ship.hpp"
#include <vector>


Laser::Laser(Game* game)
    : Actor(game)
    , lifeTime(0)
    , _sc(new SpriteComponent(this))
    , _mc(new MoveComponent(this))
{
    _sc->SetTexture(mGame->GetTexture("Assets/Laser.png"));
    _mc->SetForwardSpeed(LaserConst::Speed);
}


void Laser::OnUpdate(float deltaTime){
    lifeTime += deltaTime;
    if(lifeTime > laserCoolDown){
        this->mState = ActorState::Destroy;
    }
    else{
        const std::vector<Asteroid*> asters = mGame->GetAsteroid();
        for(auto a : asters){
            const Vector2 ap = a->GetPosition();
            const Vector2 lp = this->mPosition;
            float dist = (ap - lp).Length();
            if(dist <= LaserConst::Range){
                a->SetState(ActorState::Destroy);
                this->SetState(ActorState::Destroy);
            }
        }
    }
}
