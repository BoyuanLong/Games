//
//  Boss.hpp
//  Game-mac
//
//  Created by Sky X on 9/30/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Boss_hpp
#define Boss_hpp

#include "Actor.h"

enum BossState{
    Normal,
    Enraged
};

class Boss : public Actor{
public:
    Boss(Game* game);
    void OnUpdate(float deltaTime) override;
    void SetBossState(BossState state){ mState = state; };
    void Attack();
private:
    BossState mState;
    float attackCD;
};

#endif /* Boss_hpp */
