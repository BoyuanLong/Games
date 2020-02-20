//
//  Block.hpp
//  Game-mac
//
//  Created by Sky X on 10/20/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Block_hpp
#define Block_hpp

#include "Actor.h"

class Block : public Actor{
public:
    Block(class Game* game, bool isExplosing);
    ~Block();
    
    void OnUpdate(float deltaTime) override;
    
    void Explode();
    bool isExplodable(){return explosing; }
    
private:
    bool explosing;
};

#endif /* Block_hpp */
