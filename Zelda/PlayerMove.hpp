//
//  PlayerMove.hpp
//  Game-mac
//
//  Created by Sky X on 9/19/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef PlayerMove_hpp
#define PlayerMove_hpp

#include "MoveComponent.h"
#include "Math.h"
class Actor;
class Sword;

enum class MoveDir
{
    UP,
    RIGHT,
    LEFT,
    DOWN,
    IDLE
};

class PlayerMove : public MoveComponent{
public:
    PlayerMove(Actor* actor);
    
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    
    void UpdateSword();
    
private:
    float mSpeed;
    Vector2 direction;
    MoveDir dirState;
    MoveDir faceState;
    Sword* mSword;
    float attackTimer;
    bool lastFrameSpace;
    float invulnerableTimer;
};

#endif /* PlayerMove_hpp */
