//
//  Checkpoint.hpp
//  Game-mac
//
//  Created by Sky X on 11/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Checkpoint_hpp
#define Checkpoint_hpp

#include "Actor.h"
#include <string>

class Checkpoint : public Actor{
public:
    const Vector3 CheckpointSize = Vector3(25.0f, 25.0f, 25.0f);
public:
    Checkpoint(class Game* game);
    void OnUpdate(float deltaTime) override;
    void Activate(bool a){ active = a; }
    void SetString(std::string s){mLevelString = s;}
    std::string GetString(){ return mLevelString; }
private:
    bool active;
    std::string mLevelString;
};

#endif /* Checkpoint_hpp */
