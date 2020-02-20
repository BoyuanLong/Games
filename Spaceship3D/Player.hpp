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
    Player(class Game* game);
private:
    MeshComponent* mMeshComponent;
};

#endif /* Player_hpp */
