//
//  EnemyMove.hpp
//  Game-mac
//
//  Created by Sky X on 10/27/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef EnemyMove_hpp
#define EnemyMove_hpp

#include "VehicleMove.hpp"
#include <vector>
#include "Math.h"

class EnemyMove : public VehicleMove{
public:
    EnemyMove(class Actor* owner);
    
    void Update(float deltaTime) override;
    
private:
    std::vector<Vector3> path;
    int index;
};

#endif /* EnemyMove_hpp */
