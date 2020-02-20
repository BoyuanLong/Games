//
//  Collectible.hpp
//  Game-mac
//
//  Created by Sky X on 9/29/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Collectible_hpp
#define Collectible_hpp

#include "Component.h"
#include <functional>

class Collectible : public Component{
public:
    Collectible(Actor* owner);
    void SetCollect(std::function<void()> OnCollect){ mOnCollect = OnCollect; };
    void Update(float deltaTime) override;
private:
    std::function<void()> mOnCollect;
};

#endif /* Collectible_hpp */
