//
//  Coin.hpp
//  Game-mac
//
//  Created by Sky X on 11/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Coin_hpp
#define Coin_hpp

#include "Actor.h"

class Coin : public Actor{
public:
    const Vector3 CoinSize = Vector3(100.0f, 100.0f, 100.0f);
public:
    Coin(class Game* game);
    void OnUpdate(float deltaTime) override;
private:
    
};

#endif /* Coin_hpp */
