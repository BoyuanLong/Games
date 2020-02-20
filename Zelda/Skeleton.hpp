//
//  Skeleton.hpp
//  Game-mac
//
//  Created by Sky X on 9/29/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Skeleton_hpp
#define Skeleton_hpp

#include "Actor.h"

class Skeleton : public Actor{
public:
    Skeleton(Game* game);
    void OnUpdate(float deltaTime) override;
    
private:
    float ShootTimer;
};

#endif /* Skeleton_hpp */
