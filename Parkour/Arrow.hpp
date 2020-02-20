//
//  Arrow.hpp
//  Game-mac
//
//  Created by Sky X on 11/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Arrow_hpp
#define Arrow_hpp

#include "Actor.h"

class Arrow : public Actor{
public:
    const Vector3 Unprojection = Vector3(0.0f, 250.0f, 0.1f);
public:
    Arrow(class Game* game);
    void OnUpdate(float deltaTime) override;
    inline Quaternion GetPointingQua();
private:
    
};

#endif /* Arrow_hpp */
