//
//  Arrow.cpp
//  Game-mac
//
//  Created by Sky X on 11/21/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "Arrow.hpp"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Player.hpp"
#include "Checkpoint.hpp"


Arrow::Arrow(Game* game)
: Actor(game)
{
    Set_Mesh("Assets/Arrow.gpmesh");
    SetScale(0.15f);
}

void Arrow::OnUpdate(float deltaTime){
    //Set Quaternion
    SetQua(GetPointingQua());
    
    //Fix the position on screen
    SetPosition(mGame->GetRenderer()->Unproject(Unprojection));
}

inline Quaternion Arrow::GetPointingQua(){
    auto p = mGame->GetPlayer();
    auto cp = mGame->FrontCheck();
    
    return (cp == nullptr) ?
            Quaternion::Identity : CalculateQua(p->GetPosition(), cp->GetPosition());
}
