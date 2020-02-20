//
//  SecurityCamera.cpp
//  Game-mac
//
//  Created by Sky X on 11/27/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "SecurityCamera.hpp"
#include "Renderer.h"
#include "Game.h"
#include "MeshComponent.h"
#include "SecurityCone.hpp"
#include "Player.hpp"
#include <iostream>

#include "utils.hpp"


SecurityCamera::SecurityCamera(Game* game)
: Actor(game)
, deathClock(0.0f)
, rotClock(0.0f)
, interpTime(0.0f)
, pauseTime(0.0f)
, startQ()
, endQ()
, mCameraFX(-1)
, mCameraMotorFX(-1)
{
    Set_Mesh("Assets/Camera.gpmesh");
    sc = new SecurityCone(game, this);
}

SecurityCamera::~SecurityCamera(){
    HaltChannel(mCameraMotorFX);
}

void SecurityCamera::OnUpdate(float deltaTime){
    
    //Get Position & forward of SecurityCone
    Vector3 forward = sc->GetWorldForward();
    Vector3 position = sc->GetWorldPosition();
    Player* p = mGame->GetPlayer();
    
    Vector3 ptop = Vector3::Normalize(p->GetPosition() - position);
    float dist = (p->GetPosition() - position).Length();
    float angle = Math::Acos(Vector3::Dot(ptop, forward));
    
    
    //Camera detection check
    if(angle < Math::Pi / 6 && dist < 300.0f){ //Need to add distance check
        //Play the detected sound
        StartDetectSound();
        
        //Stop the motor sound if camera is moving
        StopMotorSound();
        
        //Change the cone texture to yellow
        sc->GetComponent<MeshComponent>()->SetTextureIndex(1);
        //Counting the time player is within the range
        deathClock += deltaTime;
        
        //If greater than 2s, change the texture to yellow
        //If greater than 2.5s, kill the player and make it respawn.
        if(deathClock > 2.0f){
            sc->GetComponent<MeshComponent>()->SetTextureIndex(2);
        }
        if(deathClock > 2.5f){
            p->Die();
        }
        
    }
    else{
        //If the player is not detected
        
        //Clear the detection status
        //  Halt the detection channel and set it to -1
        //  reset the clock to 0
        //  reset the texture back to white
        HaltChannel(mCameraFX);
        mCameraFX = -1;
        deathClock = 0.0f;
        sc->GetComponent<MeshComponent>()->SetTextureIndex(0);
        
        
        //Update the rotation timer
        rotClock += deltaTime;
        //If the timer is less than interpTime, it should be rotating
        if(rotClock <= interpTime){
            Quaternion target = Quaternion::Slerp(startQ, endQ, rotClock / interpTime);
            SetQua(target);
            StartMotorSound();
        }
        else{
            StopMotorSound();
        }
        //If the timer is bigger than interpTime and pauseTime,
        //it should start to rotate at the opposite direction and reset the timer.
        if(rotClock >= interpTime + pauseTime){
            std::swap(startQ, endQ);
            rotClock = 0.0f;
        }
    }
    
    //If camera is rotating, adjust the volume.
    if(mCameraMotorFX != -1){
        float volume = GetMotorVolume();
        Mix_Volume(mCameraMotorFX, volume);
    }
    
}

float SecurityCamera::GetMotorVolume(){
    Vector3 position = sc->GetWorldPosition();
    Player* p = mGame->GetPlayer();
    //Calculate distance between player and security camera
    float distance = (p->GetWorldPosition() - position).Length();
    
    //Calculate the volume based on a linear function
    float volume = LinearDecay(distance);
    
    //Clamp the volume so that it's within the [0, 128] range.
    volume = Math::Clamp(volume, 0.0f, 128.0f);
    return volume;
}

inline void SecurityCamera::StopMotorSound(){
    //Stop the motor sound if it's playing
    if(mCameraMotorFX != -1){
        HaltChannel(mCameraMotorFX);
        mCameraMotorFX = -1;
        Mix_PlayChannel(Mix_GroupAvailable(1), mGame->GetSound("Assets/Sounds/CameraMotorStop.wav"), 0);
    }
}

inline void SecurityCamera::StartMotorSound(){
    //If not started already, play the motor sound.
    if(mCameraMotorFX == -1){
        mCameraMotorFX = Mix_PlayChannel(Mix_GroupAvailable(1), mGame->GetSound("Assets/Sounds/CameraMotor.wav"), 0);
    }
}

inline void SecurityCamera::StartDetectSound(){
    //If not started already, play the detected sound.
    if(mCameraFX == -1){
        mCameraFX = Mix_PlayChannel(Mix_GroupAvailable(1),
                                    mGame->GetSound("Assets/Sounds/SecurityDetected.wav"), 0);
    }
}
