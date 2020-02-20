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

CameraComponent::CameraComponent(Actor* owner)
: Component(owner)
{
    //Set dampening factor
    dampening = 2.0f * Math::Sqrt(Spring);
    
    //Set to ideal at the beginning
    cameraPos = SnapToIdeal();
    
    Matrix4 lookat = Matrix4::CreateLookAt(
                    cameraPos,
                    mOwner->GetPosition() + mOwner->GetForward() * TargetDist,
                    Vector3::UnitZ);
    mOwner->GetGame()->GetRenderer()->SetViewMatrix(lookat);
}

void CameraComponent::Update(float deltaTime){
    Vector3 ideal = SnapToIdeal();
    //Adjust camera based on spring
    Vector3 displacement = cameraPos - ideal;
    Vector3 springAcc = (-Spring * displacement) - dampening * cameraVel;
    cameraVel += springAcc * deltaTime;
    cameraPos += cameraVel * deltaTime;
    
    Matrix4 lookat = Matrix4::CreateLookAt(cameraPos, mOwner->GetPosition() + mOwner->GetForward() * TargetDist, Vector3::UnitZ);
    mOwner->GetGame()->GetRenderer()->SetViewMatrix(lookat);
}

Vector3 CameraComponent::SnapToIdeal(){
    Vector3 pos = mOwner->GetPosition();
    
    return pos - mOwner->GetForward() * HDist + Vector3(0, 0, VDist);
}

