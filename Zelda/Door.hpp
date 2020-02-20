//
//  Door.hpp
//  Game-mac
//
//  Created by Sky X on 9/27/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Door_hpp
#define Door_hpp

#include "Actor.h"
#include "Math.h"
#include <string>
class Game;



enum class DoorDirection
{
    Up,
    Down,
    Left,
    Right
};

enum class DoorState
{
    Open,
    Closed,
    Locked
};



class Door : public Actor{
public:
    Door(Game* game);
    void SetUp(std::string state, std::string dir, std::string dist);
    void UpdateTexture();
    DoorState getState(){ return mState; };
    void SetState(DoorState state){ mState = state; };
    std::string GetDistination(){ return distRoom; };
private:
    DoorState mState;
    DoorDirection mDirection;
    std::string distRoom;
};

#endif /* Door_hpp */
