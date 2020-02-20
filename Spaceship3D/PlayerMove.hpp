//
//  PlayerMove.hpp
//  Game-mac
//
//  Created by Sky X on 10/20/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef PlayerMove_hpp
#define PlayerMove_hpp

#include "MoveComponent.h"
#include "Math.h"

namespace PLAYER {
const float YBound = 195.0f;
const float ZBound = 225.0f;
}

class PlayerMove : public MoveComponent{
public:
    PlayerMove(class Actor* owner);
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    Vector3 GetVelocity(){return mVelocity; }
private:
    Vector3 mVelocity;
    float vMultiplier;
    float mutiplyTimer;
    bool lastFrameSpace;
};

#endif /* PlayerMove_hpp */
