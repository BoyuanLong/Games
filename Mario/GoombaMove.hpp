//
//  GoombaMove.hpp
//  Game-mac
//
//  Created by Sky X on 9/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef GoombaMove_hpp
#define GoombaMove_hpp

#include "MoveComponent.h"
class Actor;


class GoombaMove : public MoveComponent{
public:
    GoombaMove(Actor* owner);
    void Update(float deltaTime) override;
private:
    //check if little Goombas hit each other
    //If so, both should move to a different direction
    void HitGoomba(float deltaTime);
    //check if little Goom hit a block
    //If so, it should bounce back
    void HitBlock(float deltaTime);
    //check if little Goomba is on ground
    //If not, it should fall
    void OnGround(float deltaTime);
    float yspeed;
};

#endif /* GoombaMove_hpp */
