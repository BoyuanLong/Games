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
#include "VehicleMove.hpp"
#include "Math.h"

class PlayerMove : public VehicleMove{
public:
    PlayerMove(class Actor* owner);
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    
    void OnLapChange(int newLap) override;
private:
};

#endif /* PlayerMove_hpp */
