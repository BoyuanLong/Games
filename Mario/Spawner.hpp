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

class Spawner : public Actor {
public:
    Spawner(Game* game);
    void OnUpdate(float deltaTime) override;
};

#endif /* Spawner_hpp */
