//
//  Laser.hpp
//  Game-mac
//
//  Created by Sky X on 9/7/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Laser_hpp
#define Laser_hpp

#include "Actor.h"
class Game;
class SpriteComponent;
class MoveComponent;

namespace LaserConst {
    const float Speed = 400.0f;
    const int Range = 70;
}


class Laser : public Actor{
public:
    Laser(Game* game);
    
    virtual void OnUpdate(float deltaTime) override;
    
private:
    SpriteComponent* _sc;
    MoveComponent* _mc;
    
    float lifeTime;
    
};

#endif /* Laser_hpp */
