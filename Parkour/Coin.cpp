//
//  Coin.cpp
//  Game-mac
//
//  Created by Sky X on 11/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Coin.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Player.hpp"

#include "utils.hpp"

Coin::Coin(Game* game)
: Actor(game)
{
    Set_Mesh("Assets/Coin.gpmesh");
    CollisionComponent* cc = new CollisionComponent(this);
    cc->SetSize(CoinSize);
}

void Coin::OnUpdate(float deltaTime){
    //Rotate
    SetRotation(Math::Pi * deltaTime + GetRotation());
    
    //Play sound and be destroyed after player collides with it
    if(GetComponent<CollisionComponent>()->
       Intersect(mGame->GetPlayer()->GetComponent<CollisionComponent>()))
    {
        Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Coin.wav"), 0);
        SetState(ActorState::Destroy);
    }
}
