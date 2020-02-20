//
//  LaserMine.cpp
//  Game-mac
//
//  Created by Sky X on 11/17/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "LaserMine.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Renderer.h"
#include "LaserComponent.hpp"

LaserMine::LaserMine(Game* game, Actor* parent)
: Actor(game, parent)
{
    Set_Mesh("Assets/LaserMine.gpmesh");
    new LaserComponent(this);
}
