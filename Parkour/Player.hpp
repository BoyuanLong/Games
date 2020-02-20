//
//  Player.hpp
//  Game-mac
//
//  Created by Sky X on 10/20/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include "Actor.h"

class MeshComponent;

class Player : public Actor{
public:
    const Vector3 PlayerSize = Vector3(50.0, 175.0f, 50.0f);
public:
    Player(class Game* game);
    
    void SetSpawn(Vector3 pos){ respawn = pos; }
    Vector3 GetSpawn(){ return respawn; }
    
    void Die();
private:
    Vector3 respawn;
};

#endif /* Player_hpp */
