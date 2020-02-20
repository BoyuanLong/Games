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
#include <string>
class SpriteComponent;
class CollisionComponent;
class Game;
class PlayerMove;
class AnimatedSprite;

class Player : public Actor{
public:
    Player(Game* game);
    
    CollisionComponent* GetCC(){ return _cc; };
    void SetAnimation(std::string name);
    void KeyPickUp(){++keyCount;};
    void DoorUnlock(){--keyCount;};
    int getKey(){ return keyCount; };
    void TakeDamge(int amount);
    
private:
    AnimatedSprite* _sc;
    CollisionComponent* _cc;
    PlayerMove* _pm;
    int keyCount;
    int hitPoint;
    
};

#endif /* Player_hpp */
