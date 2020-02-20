//
//  Ship.hpp
//  Game-mac
//
//  Created by Sky X on 9/6/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Ship_hpp
#define Ship_hpp

#include "Actor.h"
class SpriteComponent;
class MoveComponent;

const int Ship_SPEED = 100;
const float ANGULAR_SPEED = Math::Pi;
const int laserCoolDown = 1;

class Ship : public Actor{
public:
    Ship(Game* game);
    
    virtual void OnProcessInput(const Uint8* keyState) override;
    virtual void OnUpdate(float deltaTime) override;
    
protected:
    MoveComponent* _mc;
    SpriteComponent* _sc;
    float laserCD;
};

#endif /* Ship_hpp */
