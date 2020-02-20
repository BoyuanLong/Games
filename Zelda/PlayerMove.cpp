//
//  PlayerMove.cpp
//  Game-mac
//
//  Created by Sky X on 9/19/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "PlayerMove.hpp"
#include "SDL/SDL.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Player.hpp"
#include "AnimatedSprite.h"
#include "SDL/SDL_mixer.h"
#include "Collider.hpp"
#include "Door.hpp"
#include "SecretBlock.hpp"
#include "Sword.hpp"
#include "EnemyComponent.hpp"
#include "Boss.hpp"

const int WINW = 600;
const int WINH = 448;

PlayerMove::PlayerMove(Actor* actor)
: MoveComponent(actor)
, mSpeed(130.0f)
, direction(0, 0)
, mSword(new Sword(mOwner->GetGame()))
, attackTimer(0.0f)
, lastFrameSpace(false)
, invulnerableTimer(0)
{
    
}


void PlayerMove::ProcessInput(const Uint8* keyState){
    //Get Moving direction
    if(keyState[SDL_SCANCODE_LEFT]){
        direction = Vector2(-1, 0);
        dirState = MoveDir::LEFT;
        faceState = dirState;
    }
    else if(keyState[SDL_SCANCODE_RIGHT]){
        direction = Vector2(1, 0);
        dirState = MoveDir::RIGHT;
        faceState = dirState;
    }
    else if(keyState[SDL_SCANCODE_UP]){
        direction = Vector2(0, -1);
        dirState = MoveDir::UP;
        faceState = dirState;
    }
    else if(keyState[SDL_SCANCODE_DOWN]){
        direction = Vector2(0, 1);
        dirState = MoveDir::DOWN;
        faceState = dirState;
    }
    else{
        direction = Vector2(0, 0);
        dirState = MoveDir::IDLE;
    }
    
    if(keyState[SDL_SCANCODE_SPACE] && !lastFrameSpace){
        attackTimer = 0.25f;
        UpdateSword();
    }
    lastFrameSpace = keyState[SDL_SCANCODE_SPACE];
    
}

void PlayerMove::Update(float deltaTime){
    invulnerableTimer -= deltaTime;
    //Fix Camera location
    Vector2 linkPosition = mOwner->GetPosition();
    mOwner->GetGame()->SetCamera(linkPosition - Vector2(WINW / 2, WINH / 2));
    switch (dirState) {
        case MoveDir::IDLE:
            reinterpret_cast<Player*>(mOwner)->SetAnimation("pause");;
            break;
        case MoveDir::LEFT:
            reinterpret_cast<Player*>(mOwner)->SetAnimation("walkLeft");
            break;
        case MoveDir::RIGHT:
            reinterpret_cast<Player*>(mOwner)->SetAnimation("walkRight");
            break;
        case MoveDir::UP:
            reinterpret_cast<Player*>(mOwner)->SetAnimation("walkUp");
            break;
        case MoveDir::DOWN:
            reinterpret_cast<Player*>(mOwner)->SetAnimation("walkDown");
            break;
        default:
            break;
    }
    if(attackTimer > 0){
        attackTimer -= deltaTime;
    }
    else{
        mOwner->SetPosition(mOwner->GetPosition() + direction * mSpeed * deltaTime);
    }
    
    
    std::string currentRoom = mOwner->GetGame()->GetCurrentRoom();
    std::vector<Door*> doors = mOwner->GetGame()->GetDoors(currentRoom);
    
    Vector2 dumb;
    for(Door* d : doors){
        CollSide side = mOwner->GetComponent<CollisionComponent>()->GetMinOverlap(d->GetComponent<CollisionComponent>(), dumb);
        if(side != CollSide::None && d->getState() == DoorState::Open){
            if(side == CollSide::Left && dirState == MoveDir::LEFT){
                mOwner->SetPosition(mOwner->GetPosition() - Vector2(d->GetComponent<CollisionComponent>()->GetWidth() * 2, 0));
                mOwner->GetGame()->SetCurrentRoom(d->GetDistination());
            }
            else if(side == CollSide::Right && dirState == MoveDir::RIGHT){
                mOwner->SetPosition(mOwner->GetPosition() + Vector2(d->GetComponent<CollisionComponent>()->GetWidth() * 2, 0));
                mOwner->GetGame()->SetCurrentRoom(d->GetDistination());
            }
            else if(side == CollSide::Top && dirState == MoveDir::UP){
                mOwner->SetPosition(mOwner->GetPosition() - Vector2(0, d->GetComponent<CollisionComponent>()->GetHeight() * 2));
                mOwner->GetGame()->SetCurrentRoom(d->GetDistination());
            }
            else if(side == CollSide::Bottom && dirState == MoveDir::DOWN){
                mOwner->SetPosition(mOwner->GetPosition() + Vector2(0, d->GetComponent<CollisionComponent>()->GetHeight() * 2));
                mOwner->GetGame()->SetCurrentRoom(d->GetDistination());
            }
        }
        else if(side != CollSide::None && d->getState() == DoorState::Locked
                && reinterpret_cast<Player*>(mOwner)->getKey() > 0){
            d->SetState(DoorState::Open);
            Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/DoorOpen.wav"), 0);
            d->UpdateTexture();
            reinterpret_cast<Player*>(mOwner)->DoorUnlock();
        }
    }
    
    std::vector<Actor*> enemies = mOwner->GetGame()->GetEnemies();
    for(Actor* e : enemies){
        if(attackTimer > 0){
            if(mSword->GetComponent<CollisionComponent>()->
               Intersect(e->GetComponent<CollisionComponent>())){
                e->GetComponent<EnemyComponent>()->TakeDamage(1);
            }
        }
        else if(invulnerableTimer <= 0){
            Vector2 offset;
            CollSide side = mOwner->GetComponent<CollisionComponent>()->
                GetMinOverlap(e->GetComponent<CollisionComponent>(), offset);
            if(side != CollSide::None){
                mOwner->SetPosition(mOwner->GetPosition() + offset);
                reinterpret_cast<Player*>(mOwner)->TakeDamge(1);
                mOwner->SetPosition(mOwner->GetPosition() - direction * 32);
                invulnerableTimer = 0.5;
            }
        }
    }
    
    
    
    Vector2 offset;
    std::vector<Collider*> colliders = mOwner->GetGame()->GetColliders();
    for(Collider* c : colliders){
        CollSide side = mOwner->GetComponent<CollisionComponent>()->GetMinOverlap(c->GetComponent<CollisionComponent>(), offset);
        if(side != CollSide::None){
            mOwner->SetPosition(mOwner->GetPosition() + offset);
            //break;
        }
    }
    
    std::vector<SecretBlock*> secrets = mOwner->GetGame()->getSecrets();
    for(SecretBlock* s : secrets){
        CollSide side = mOwner->GetComponent<CollisionComponent>()->GetMinOverlap(s->GetComponent<CollisionComponent>(), offset);
        mOwner->SetPosition(mOwner->GetPosition() + offset);
        if(side == s->getDirection() &&
           ((dirState == MoveDir::UP && side == CollSide::Top) ||
            (dirState == MoveDir::LEFT && side == CollSide::Left) ||
            (dirState == MoveDir::RIGHT && side == CollSide::Right) ||
            (dirState == MoveDir::DOWN && side == CollSide::Bottom)
            )){
            s->Move(deltaTime, direction);
        }
    }
    UpdateSword();
}

void PlayerMove::UpdateSword(){
    if(attackTimer <= 0){
        mSword->GetComponent<SpriteComponent>()->SetIsVisible(false);
    }
    else{
        Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/SwordSlash.wav"), 0);
        SpriteComponent* swordSC = mSword->GetComponent<SpriteComponent>();
        Game* game = mOwner->GetGame();
        if(faceState  == MoveDir::UP){
            swordSC->SetTexture(game->GetTexture("Assets/Sword/Up.png"));
            mSword->SetPosition(mOwner->GetPosition() + Vector2(-8, -16));
            reinterpret_cast<Player*>(mOwner)->SetAnimation("attackUp");
        }
        else if(faceState == MoveDir::RIGHT){
            swordSC->SetTexture(game->GetTexture("Assets/Sword/Right.png"));
            mSword->SetPosition(mOwner->GetPosition() + Vector2(16, 4));
            reinterpret_cast<Player*>(mOwner)->SetAnimation("attackRight");
        }
        else if(faceState == MoveDir::LEFT){
            swordSC->SetTexture((game->GetTexture("Assets/Sword/Left.png")));
            mSword->SetPosition(mOwner->GetPosition() + Vector2(-16, 4));
            reinterpret_cast<Player*>(mOwner)->SetAnimation("attackLeft");
        }
        else if(faceState == MoveDir::DOWN){
            swordSC->SetTexture(game->GetTexture("Assets/Sword/Down.png"));
            mSword->SetPosition(mOwner->GetPosition() + Vector2(8, 16));
            reinterpret_cast<Player*>(mOwner)->SetAnimation("attackDown");
        }
        swordSC->SetIsVisible(true);
    }
}
