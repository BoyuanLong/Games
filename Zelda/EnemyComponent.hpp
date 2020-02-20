//
//  EnemyComponent.hpp
//  Game-mac
//
//  Created by Sky X on 9/29/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef EnemyComponent_hpp
#define EnemyComponent_hpp

#include "Component.h"
#include <functional>
class Actor;

class EnemyComponent : public Component{
public:
    EnemyComponent(Actor* owner);
    ~EnemyComponent();
    void SetOnDeath(std::function<void()> Ondeath){ mOnDeath = Ondeath; };
    void SetOnTakeDamge(std::function<void()> takedamage){ mOnTakeDamge = takedamage; };
    void TakeDamage(int amount);
    void SetHP(int hp){ hitPoint = hp; };
    void setDamge(int d){ damage = d; };
    int GetHP(){ return hitPoint; };
    void SetBoss(){ isBoss = true; };
    void Update(float deltaTime) override;
private:
    int hitPoint;
    int damage;
    std::function<void()> mOnDeath;
    std::function<void()> mOnTakeDamge;
    float invulnerableTimer;
    bool isBoss;
};

#endif /* EnemyComponent_hpp */
