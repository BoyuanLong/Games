//
//  Spawner.cpp
//  Game-mac
//
//  Created by Sky X on 9/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Spawner.hpp"

#include "Game.h"

Spawner::Spawner(Game* game, std::string type)
: Actor(game)
, mType(type){
    
}

