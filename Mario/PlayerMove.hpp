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
class Actor;

class PlayerMove : public MoveComponent{
public:
    PlayerMove(Actor* actor);
    
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    
private:
    //Check Mario's Position to see if he wins or goes out of boundry
    void Stalker();
    //Handle Blocks offset if touches the blocks
    //Return if Mario is standing on any block
    bool CollideWithBlocks(float deltaTime);
    //Check if Mario stomped on any goombas
    void StompGoombas(float deltaTime);
    //Handle all Animation Mario has
    void AnimationManager();
    float mYSpeed;
    bool mSpacePressed;
    bool mInAir;
};

#endif /* PlayerMove_hpp */
