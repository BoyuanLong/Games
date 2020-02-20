//
//  Collider.hpp
//  Game-mac
//
//  Created by Sky X on 9/26/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Collider_hpp
#define Collider_hpp

#include "Actor.h"
class CollisionComponent;
class Game;

class Collider : public Actor{
public:
    Collider(Game* game);
    void SetDimention(float width, float height);
private:
    CollisionComponent* _cc;
    float mWidth;
    float mHeight;
};

#endif /* Collider_hpp */
