//
//  SecurityCone.cpp
//  Game-mac
//
//  Created by Sky X on 11/27/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "SecurityCone.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"

SecurityCone::SecurityCone(Game* game, Actor* parent)
: Actor(game, parent)
{
    Set_Mesh("Assets/Cone.gpmesh", true);
    SetPosition(ChildOffset);
    SetScale(ConeScale);
}
