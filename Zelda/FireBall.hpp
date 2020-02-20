//
//  FireBall.hpp
//  Game-mac
//
//  Created by Sky X on 9/29/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef FireBall_hpp
#define FireBall_hpp

#include "Actor.h"
#include <string>
class Game;

class FireBall : public Actor{
public:
    FireBall(Game* game);
    FireBall(Game* game, std::string color);
    void OnUpdate(float deltaTime) override;
private:
    float deathTimer;
};

#endif /* FireBall_hpp */
