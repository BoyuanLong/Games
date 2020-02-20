//
//  VehicleMove.cpp
//  Game-mac
//
//  Created by Sky X on 10/26/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "VehicleMove.hpp"
#include "Actor.h"
#include "HeightMap.hpp"
#include "Game.h"
#include <iostream>
#include <fstream>
#include "CSVHelper.h"

using std::vector;
using std::pair;

VehicleMove::VehicleMove(Actor* owner)
: Component(owner, 50)
, mAccelerated(false)
, mTurnDirection(Direction::None)
, mVelocity()
, mAngularVelocity(0)
, timeAcelerated(0)
, lap(0)
, lastPoint(-1)
{
    std::fstream infile("Assets/HeightMap/Checkpoints.csv");
    std::string line;
    std::getline(infile, line);
    while(std::getline(infile, line)){
        vector<std::string> elements = CSVHelper::Split(line);
        pair<int, int> min = std::make_pair(std::stoi(elements[1]), std::stoi(elements[3]));
        pair<int, int> max = std::make_pair(std::stoi(elements[2]), std::stoi(elements[4]));
        minCells.emplace_back(min);
        maxCells.emplace_back(max);
    }
}

void VehicleMove::UpdateVelocity(float deltaTime){
    //Get the previous position
    Vector3 updatePos = mOwner->GetPosition();
    
    if(mAccelerated){
        //If pedal is pressed, calculate acceleration and increase
        //the velocity
        timeAcelerated += deltaTime;
        float acceleratation = Math::Lerp(MinAcc,
                                          MaxAcc,
                                std::min(timeAcelerated/RampTime, 1.0f));
        mVelocity += mOwner->GetForward() * acceleratation * deltaTime;
    }
    else{
        timeAcelerated = 0.0f;
    }

    updatePos += mVelocity * deltaTime;
    
    //Friction
    mVelocity *= (mAccelerated ? Drag : StaticDrag);
    
    //Adjust the z-value according to the map height
    HeightMap* map = mOwner->GetGame()->GetHeightMap();
    if(map->IsOnTrack(updatePos.x, updatePos.y)){
        updatePos.z = Math::Lerp(updatePos.z, map->GetHeight(updatePos.x, updatePos.y), 0.1f) ;
    }
    
    mOwner->SetPosition(updatePos);
}

void VehicleMove::UpdateTurn(float deltaTime){
    //Set turnFactor
    //-1 if left
    //1 if right
    int turnFactor = 0;
    if(mTurnDirection != Direction::None){
        turnFactor = (mTurnDirection == Direction::Left ? -1 : 1);
        mAngularVelocity += AngularAcc * turnFactor * deltaTime;
    }
    
    //Set Rotation
    mOwner->SetRotation(mOwner->GetRotation() + mAngularVelocity * deltaTime);
    
    //Exert friction
    mAngularVelocity *= AngularDrag;
}



void VehicleMove::Update(float deltaTime){
    UpdateVelocity(deltaTime);
    UpdateTurn(deltaTime);
    
    
    int nextPoint = (lastPoint + 1) % minCells.size();
    
    float x = mOwner->GetPosition().x;
    float y = mOwner->GetPosition().y;
    int row = 0;
    int col = 0;
    mOwner->GetGame()->GetHeightMap()->WorldToCell(x, y, row, col);
    
    int minR = minCells[nextPoint].first;
    int minC = minCells[nextPoint].second;
    int maxR = maxCells[nextPoint].first;
    int maxC = maxCells[nextPoint].second;
    
    if(row <= maxR && row >= minR && col <= maxC && col >= minC){
        lastPoint = (lastPoint + 1) % (int)minCells.size();
        if(lastPoint == 0){
            ++lap;
            OnLapChange(lap);
        }
    }
}


Vector3 VehicleMove::GetNextLocation(){
    int nextPoint = (lastPoint + 1) % (int)minCells.size();
    
    int minR = (minCells[nextPoint].first + maxCells[nextPoint].first)/2;
    int minC = (minCells[nextPoint].second + maxCells[nextPoint].second) /2;
    
    return mOwner->GetGame()->GetHeightMap()->CellToWorld(minR, minC);
}
