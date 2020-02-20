//
//  SecurityCone.hpp
//  Game-mac
//
//  Created by Sky X on 11/27/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef SecurityCone_hpp
#define SecurityCone_hpp

#include "Actor.h"

class SecurityCone : public Actor{
public:
    const float ConeScale = 3.0f;
    //SecurityCone position in parent's space
    const Vector3 ChildOffset = Vector3(30.0f, 0.0f, 0.0f);
public:
    SecurityCone(class Game* game, Actor* parent = nullptr);
    
private:
    
    
};

#endif /* SecurityCone_hpp */
