//
//  CameraComponent.cpp
//  Game-mac
//
//  Created by Sky X on 10/27/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "CameraComponent.hpp"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Math.h"

CameraComponent::CameraComponent(Actor* owner)
: Component(owner)
, mPitchAngle(0.0f)
, mPitchSpeed(0.0f)
{
    mRotationTimer = 0.0f;
    mTargetAngle = Vector3::UnitZ;
    mCurrentAngle = Vector3::UnitZ;
    mStartAngle = Vector3::UnitZ;
}

void CameraComponent::Update(float deltaTime){

    
    mPitchAngle += mPitchSpeed * deltaTime;
    mPitchAngle = Math::Clamp(mPitchAngle, -Math::PiOver2 / 2, Math::PiOver2 / 2);
    Matrix4 pitch = Matrix4::CreateRotationY(mPitchAngle);
    Matrix4 yaw = Matrix4::CreateRotationZ(mOwner->GetRotation());
    Matrix4 combined = pitch * yaw;
    Vector3 transfomed = Vector3::Transform(Vector3(1, 0, 0), combined);
    mRotationTimer += deltaTime;
    mCurrentAngle = Vector3::Lerp(mStartAngle, mTargetAngle, Math::Min(1.0f, mRotationTimer));
    
    Matrix4 lookat = Matrix4::CreateLookAt(mOwner->GetPosition(), mOwner->GetPosition() + transfomed * TargetDist, mCurrentAngle);
    mOwner->GetGame()->GetRenderer()->SetViewMatrix(lookat);
}


void CameraComponent::ChangeTarget(Vector3 offset){
    mTargetAngle = Vector3::UnitZ + offset * 0.1;
    mTargetAngle.Normalize();
    mStartAngle = mCurrentAngle;
    mRotationTimer = 0.0f;
}
