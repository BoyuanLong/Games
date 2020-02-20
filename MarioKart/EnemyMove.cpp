//
//  EnemyMove.cpp
//  Game-mac
//
//  Created by Sky X on 10/27/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "EnemyMove.hpp"
#include "Actor.h"
#include "CSVHelper.h"
#include <iostream>
#include <fstream>
#include <string>
#include "HeightMap.hpp"
#include "Game.h"

using std::fstream;
using std::vector;
using std::string;


EnemyMove::EnemyMove(Actor* owner)
: VehicleMove(owner)
, index(0)
{
    fstream infile("Assets/HeightMap/Path.csv");
    string line;
    std::getline(infile, line);
    while(std::getline(infile, line)){
        vector<string> element = CSVHelper::Split(line);
        int x = std::stoi(element[1]);
        int y = std::stoi(element[2]);
        Vector3 pos = owner->GetGame()->GetHeightMap()->CellToWorld(x, y);
        path.emplace_back(pos);
    }
    
    mOwner->SetPosition(path[index]);
    ++index;
}

void EnemyMove::Update(float deltaTime){
    Vector3 diff = path[index] - mOwner->GetPosition();
    if(Math::Abs(diff.Length()) < 100.0f){
        index = (++index) % path.size();
    }
    diff.Normalize();
    
    float dot = Vector3::Dot(mOwner->GetForward(), diff);
    
    SetPedal(dot > 0.7f);
    
    
    if(dot > 0.99f){
        SetDir(Direction::None);
    }
    else{
        if(Vector3::Cross(mOwner->GetForward(), diff).z > 0.0f){
            SetDir(Direction::Right);
        }
        else{
            SetDir(Direction::Left);
        }
    }
    
    VehicleMove::Update(deltaTime);
}
