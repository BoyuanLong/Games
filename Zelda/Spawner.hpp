//
//  Spawner.hpp
//  Game-mac
//
//  Created by Sky X on 9/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Spawner_hpp
#define Spawner_hpp

#include "Actor.h"
#include <string>

class Spawner : public Actor {
public:
    Spawner(Game* game, std::string type);
    std::string GetType(){ return mType; };
private:
    std::string mType;
};

#endif /* Spawner_hpp */
