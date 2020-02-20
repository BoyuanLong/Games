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
    
    void SetPitchAngle(float angle){ mPitchAngle = angle; }
    void SetPitchSpeed(float speed){ mPitchSpeed = speed; }
    void SetRotationAngle(Vector3 angle){ mTargetAngle = angle; }
    Vector3 GetRotateAngle(){ return mTargetAngle; }
    void ChangeTarget(Vector3 offset);
    float GetPitchAngle(){ return mPitchAngle; }
    float GetPitchSpeed(){ return mPitchSpeed; }
    
    
private:

    const float TargetDist = 75.0f;
    
    float mPitchAngle;
    float mPitchSpeed;
    Vector3 mTargetAngle;
    Vector3 mStartAngle;
    Vector3 mCurrentAngle;
    float mRotateSpeed = 70.0f;
    float mRotationTimer;

    Vector3 cameraPos;
};

#endif /* CameraComponent_hpp */
