//
//  Bat.hpp
//  Game-mac
//
//  Created by Sky X on 9/29/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Bat_hpp
#define Bat_hpp

#include "Actor.h"
class Game;

class Bat : public Actor{
public:
    Bat(Game* game);
private:
    void OnUpdate(float deltaTime) override;
    float timeremain;
    Vector2 direction;
};

#endif /* Bat_hpp */
