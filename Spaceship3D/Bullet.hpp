//
//  Bullet.hpp
//  Game-mac
//
//  Created by Sky X on 10/20/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Bullet_hpp
#define Bullet_hpp

#include "Actor.h"

namespace BULLET {
const float Speed = 500.0f;
const float Clock = 1.0f;
}

class Bullet : public Actor{
public:
    Bullet(class Game* game);
    
    void OnUpdate(float deltaTime) override;
    
private:
    float lifeTime;
};

#endif /* Bullet_hpp */
