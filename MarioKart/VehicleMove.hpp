//
//  VehicleMove.hpp
//  Game-mac
//
//  Created by Sky X on 10/26/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef VehicleMove_hpp
#define VehicleMove_hpp

#include "Component.h"
#include "Math.h"
#include <vector>

class VehicleMove : public Component{
public:
    enum Direction{
        None,
        Left,
        Right
    };
    
    VehicleMove(class Actor* owner);
    void Update(float deltaTime) override;
    
    void SetPedal(bool pedal){ mAccelerated = pedal; }
    bool GetPedal(){ return mAccelerated; }
    void SetDir(Direction dir){ mTurnDirection = dir; }
    Direction GetDir(){ return mTurnDirection; }
    
    virtual void OnLapChange(int newLap){};
    
    int GetLap(){ return lap; }
    int GetLastPoint(){ return lastPoint; }
    
    Vector3 GetNextLocation();
    
private:
    void UpdateVelocity(float deltaTime);
    void UpdateTurn(float deltaTime);
    
    bool mAccelerated;
    Direction mTurnDirection;
    Vector3 mVelocity;
    float mAngularVelocity;
    float timeAcelerated;
    
    std::vector<std::pair<int, int> > minCells;
    std::vector<std::pair<int, int> > maxCells;
    
    int lap;
    int lastPoint;
    
    const float MinAcc = 1000.0f;
    const float MaxAcc = 2500.0f;
    const float RampTime = 2.0f;
    const float AngularAcc = 5 * Math::Pi;
    const float Drag = 0.9f;
    const float StaticDrag = 0.975f;
    const float AngularDrag = 0.9f;
};

#endif /* VehicleMove_hpp */
