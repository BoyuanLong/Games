//
//  SideBlock.hpp
//  Game-mac
//
//  Created by Sky X on 10/20/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef SideBlock_hpp
#define SideBlock_hpp

#include "Actor.h"

class SideBlock : public Actor{
public:
    SideBlock(class Game* game, int count);
    void OnUpdate(float deltaTime) override;
    
private:
    int textureArr[4] = {1,2,4,2};
};

#endif /* SideBlock_hpp */
