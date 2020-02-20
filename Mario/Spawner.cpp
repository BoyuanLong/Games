//
//  Spawner.cpp
//  Game-mac
//
//  Created by Sky X on 9/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Spawner.hpp"

#include "Game.h"
#include "Goomba.hpp"

Spawner::Spawner(Game* game) : Actor(game){
    
}

void Spawner::OnUpdate(float deltaTime){
    //Spawn a goomba if Mario is WINW of the spawner
    if(GetPosition().x - mGame->GetMarioPosition().x < GAME::WINW){
        Goomba* temp = new Goomba(mGame);
        temp->SetPosition(GetPosition());
        this->SetState(ActorState::Destroy);
    }
}
