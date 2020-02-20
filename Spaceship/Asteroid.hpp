//
//  Asteroid.hpp
//  Game-mac
//
//  Created by Sky X on 9/7/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Asteroid_hpp
#define Asteroid_hpp

#include "Actor.h"
class Game;
class SpriteComponent;
class MoveComponent;

const int LOWER_MARGIN = -15;
const int X_UPPER_MARGIN = 1040;
const int Y_UPPER_MARGIN = 780;
const float SPEED = 150.0f;

class Asteroid : public Actor{
public:
    Asteroid(Game* game);
    ~Asteroid();
    virtual void OnUpdate(float deltaTime) override;
    
private:
    SpriteComponent* _sc;
    MoveComponent* _mc;
};

#endif /* Asteroid_hpp */
