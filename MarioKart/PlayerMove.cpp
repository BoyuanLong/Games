//
//  PlayerMove.cpp
//  Game-mac
//
//  Created by Sky X on 10/20/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "PlayerMove.hpp"
#include "Actor.h"
#include "SDL/SDL.h"
#include "Renderer.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "HeightMap.hpp"
#include "PlayerUI.h"
#include "Enemy.hpp"
#include "EnemyMove.hpp"

PlayerMove::PlayerMove(Actor* owner)
: VehicleMove(owner)
{
    owner->SetPosition(mOwner->GetGame()->GetHeightMap()->CellToWorld(39, 58));
}

void PlayerMove::Update(float deltaTime){
    
    VehicleMove::Update(deltaTime);
    
}

void PlayerMove::ProcessInput(const Uint8* keyState){
    if(keyState[SDL_SCANCODE_W]){
        SetPedal(true);
    }
    else{
        SetPedal(false);
    }
    
    if((keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]) && !(keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT])){
        SetDir(Direction::Left);
    }
    else if((keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) &&
            !(keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT])){
        SetDir(Direction::Right);
    }
    else{
        SetDir(Direction::None);
    }
    
}

void PlayerMove::OnLapChange(int newLap){
    PlayerUI* ui = mOwner->GetComponent<PlayerUI>();
    if(newLap == 5){
        int& bgm = mOwner->GetGame()->GetBgm();
        Mix_FadeOutChannel(bgm, 250);
        Enemy* e = mOwner->GetGame()->GetEnemy();
        if(e->GetComponent<EnemyMove>()->GetLap() == 4){
            ui->SetRaceState(PlayerUI::Won);
            Mix_FadeInChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Won.wav"), -1, 4000);
        }
        else{
            ui->SetRaceState(PlayerUI::Lost);
            Mix_FadeInChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Lost.wav"), -1, 4000);
        }
        e->SetState(ActorState::Paused);
        mOwner->SetState(ActorState::Paused);
        
        
        
    }
    else{
        ui->OnLapChange(newLap);
        if(newLap == 4){
            int& bgm = mOwner->GetGame()->GetBgm();
            Mix_FadeOutChannel(bgm, 250);
            Mix_PlayChannel(2, mOwner->GetGame()->GetSound("Assets/Sounds/FinalLap.wav"), 0);
            bgm = Mix_FadeInChannel(3, mOwner->GetGame()->GetSound("Assets/Sounds/MusicFast.ogg"), -1, 4000);
        }
    }
    
    
}
