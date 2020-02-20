//
//  Goomba.hpp
//  Game-mac
//
//  Created by Sky X on 9/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Goomba_hpp
#define Goomba_hpp

#include "Actor.h"
class SpriteComponent;
class CollisionComponent;
class MoveComponent;
class Game;
class GoombaMove;
class AnimatedSprite;


namespace GOOMBA {
const float SPEED = 100.0f;
const float DeathTimer = 0.25f;
const float GoombaSizeX = 32.0f;
const float GoombaSizeY = 32.0f;
}

class Goomba : public Actor{
public:
    Goomba(Game* game);
    ~Goomba();
    CollisionComponent* GetCC(){ return _cc; };
    GoombaMove* GetGM(){ return _gm; };
    void OnUpdate(float deltaTime);
    void Stomped(){ isStomped = true; };
    bool GetStomped(){ return isStomped; };
private:
    AnimatedSprite* _sc;
    CollisionComponent* _cc;
    GoombaMove* _gm;
    bool isStomped;
    float deathTimer;
};

#endif /* Goomba_hpp */
