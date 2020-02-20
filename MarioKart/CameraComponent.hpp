//
//  CameraComponent.hpp
//  Game-mac
//
//  Created by Sky X on 10/27/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef CameraComponent_hpp
#define CameraComponent_hpp

#include "Component.h"
#include "Math.h"

class CameraComponent : public Component{
public:
    CameraComponent(class Actor* owner);
    void Update(float deltaTime) override;
    Vector3 SnapToIdeal();
    
private:
    float HorizontalDist = 60.0f;
    float TargetOffset = 50.0f;
    float Spring = 256.0f;
    
    const float HDist = 60.0f;
    const float VDist = 70.0f;
    const float TargetDist = 75.0f;
    float dampening;
    Vector3 cameraPos;
    Vector3 cameraVel;
};

#endif /* CameraComponent_hpp */
