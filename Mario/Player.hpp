//
//  Player.hpp
//  Game-mac
//
//  Created by Sky X on 9/19/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp
#include "Actor.h"
class SpriteComponent;
class CollisionComponent;
class Game;
class PlayerMove;
class AnimatedSprite;

namespace PLAYER {
const float SPEED = 300.0f;
const float JumpSpeed = -700.0f;
const float MarioSizeX = 32.0f;
const float MarioSizeY = 32.0f;
}

class Player : public Actor{
public:
    Player(Game* game);
    
    CollisionComponent* GetCC(){ return _cc; };
    
private:
    AnimatedSprite* _sc;
    CollisionComponent* _cc;
    PlayerMove* _pm;
};

#endif /* Player_hpp */
