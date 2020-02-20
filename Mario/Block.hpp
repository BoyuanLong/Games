//
//  Block.hpp
//  Game-mac
//
//  Created by Sky X on 9/12/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Block_hpp
#define Block_hpp

#include "SDL/SDL.h"
#include "Actor.h"

class SpriteComponent;
class CollisionComponent;

namespace BLOCK {
const float BlockSizeX = 32.0f;
const float BlockSizeY = 32.0f;
}

class Block : public Actor{
public:
    Block(Game* game);
    ~Block();
    void changeTexture(SDL_Texture* texture);
    CollisionComponent* GetCC() {return _cc; };
    
private:
    SpriteComponent* _sc;
    CollisionComponent* _cc;
};

#endif /* Block_hpp */
