//
//  SecretBlock.hpp
//  Game-mac
//
//  Created by Sky X on 9/28/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef SecretBlock_hpp
#define SecretBlock_hpp

#include "Actor.h"
#include <string>
#include "Math.h"
#include "CollisionComponent.h"
class Game;

namespace SECRETBLOCK {
const int Range = 32;
const float SpeedLimit = 60.0f;
}

class SecretBlock : public Actor{
public:
    SecretBlock(Game* game, std::string direction, Vector2 origin);
    CollSide getDirection() const { return mDirection; } ;
    void Move(float deltaTime, const Vector2& dir);
private:
    CollSide getDir(const Vector2& dir);
    CollSide mDirection;
    Vector2 mOrigin;
};

#endif /* SecretBlock_hpp */
