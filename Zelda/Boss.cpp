//
//  Boss.cpp
//  Game-mac
//
//  Created by Sky X on 9/30/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Boss.hpp"
#include "Game.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "EnemyComponent.hpp"
#include "FireBall.hpp"

Boss::Boss(Game* game)
: Actor(game)
, attackCD(1.0f)
{
    AnimatedSprite* as = new AnimatedSprite(this);
    std::vector<SDL_Texture*>
    idle = {
        mGame->GetTexture("Assets/Dragon/Idle0.png"),
        mGame->GetTexture("Assets/Dragon/Idle1.png")
    },
    attack = {
        mGame->GetTexture("Assets/Dragon/Attack0.png"),
        mGame->GetTexture("Assets/Dragon/Attack1.png")
    },
    enraged = {
        mGame->GetTexture("Assets/Dragon/Enraged0.png"),
        mGame->GetTexture("Assets/Dragon/Enraged1.png"),
        mGame->GetTexture("Assets/Dragon/Enraged2.png"),
        mGame->GetTexture("Assets/Dragon/Enraged3.png")
    };
    
    as->AddAnimation("idle", idle);
    as->AddAnimation("attack", attack);
    as->AddAnimation("enraged", enraged);
    
    //Set starting animation
    as->SetAnimation("idle");
    
    CollisionComponent* cc = new CollisionComponent(this);
    cc->SetSize(50.0f, 64.0f);
    EnemyComponent* ec = new EnemyComponent(this);
    ec->SetHP(5);
    ec->setDamge(2);
    ec->SetBoss();

}

void Boss::OnUpdate(float deltaTime){
    attackCD -= deltaTime;
    if(GetComponent<EnemyComponent>()->GetHP() <= 2)
    {
        SetBossState(BossState::Enraged);
    }
    Attack();
}

void Boss::Attack(){
    switch (mState) {
        case BossState::Normal:
            if(attackCD < 0.25){
                GetComponent<AnimatedSprite>()->SetAnimation("attack");
            }
            if(attackCD <= 0){
                FireBall* fb1 = new FireBall(mGame, "Green");
                FireBall* fb2 = new FireBall(mGame, "Green");
                FireBall* fb3 = new FireBall(mGame, "Green");
                Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/Fireball.wav"), 0);
                fb1->SetRotation(atan2(1.0f, -1.732f));
                fb2->SetRotation(atan2(-1.0f, -1.732f));
                fb3->SetRotation(atan2(0.0f, -1.0f));
                fb1->SetPosition(GetPosition());
                fb2->SetPosition(GetPosition());
                fb3->SetPosition(GetPosition());
                attackCD = 2;
            }
            if(attackCD > 0.25f){
                GetComponent<AnimatedSprite>()->SetAnimation("idle");
            }
            break;
            
        case BossState::Enraged:
            GetComponent<AnimatedSprite>()->SetAnimation("enraged");
            if(attackCD <= 0){
                FireBall* fb1 = new FireBall(mGame, "Green");
                FireBall* fb2 = new FireBall(mGame, "Green");
                FireBall* fb3 = new FireBall(mGame, "Green");
                FireBall* fb4 = new FireBall(mGame, "Green");
                FireBall* fb5 = new FireBall(mGame, "Green");
                FireBall* fb6 = new FireBall(mGame, "Green");
                FireBall* fb7 = new FireBall(mGame, "Green");
                FireBall* fb8 = new FireBall(mGame, "Green");
                FireBall* fb9 = new FireBall(mGame, "Green");
                Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/Fireball.wav"), 0);
                fb1->SetRotation(atan2(1.0f, -1.732f));
                fb2->SetRotation(atan2(-1.0f, -1.732f));
                fb3->SetRotation(atan2(0.0f, -1.0f));
                
                fb4->SetRotation(atan2(-1.732f, -1.0f));
                fb5->SetRotation(atan2(1.732f, -1.0f));
                fb6->SetRotation(atan2(1.0f, 0.0f));
                
                fb7->SetRotation(atan2(0.26f, -1.0f));
                fb8->SetRotation(atan2(-0.26f, -1.0f));
                fb9->SetRotation(atan2(-1.0f, 0.0f));
                fb1->SetPosition(GetPosition());
                fb2->SetPosition(GetPosition());
                fb3->SetPosition(GetPosition());
                fb4->SetPosition(GetPosition());
                fb5->SetPosition(GetPosition());
                fb6->SetPosition(GetPosition());
                fb7->SetPosition(GetPosition());
                fb8->SetPosition(GetPosition());
                fb9->SetPosition(GetPosition());
                attackCD = 1;
            }
            
        default:
            break;
    }
}
