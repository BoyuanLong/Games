//
//  SecurityCamera.hpp
//  Game-mac
//
//  Created by Sky X on 11/27/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef SecurityCamera_hpp
#define SecurityCamera_hpp

#include "Actor.h"


class SecurityCamera : public Actor{
public:
    SecurityCamera(class Game* game);
    ~SecurityCamera();
    
    void OnUpdate(float deltaTime) override;
    
    void SetStart(Quaternion q){ startQ = q; }
    void SetEnd(Quaternion q){ endQ = q; }
    void SetPause(float f){ pauseTime = f; }
    void SetInterp(float f){ interpTime = f; }
    
private:
    class SecurityCone* sc;
    
    //Clock for detection countdown and rotation
    float deathClock;
    float rotClock;
    //Given time for interp and pause
    float interpTime;
    float pauseTime;
    
    //Rotation range
    Quaternion startQ;
    Quaternion endQ;
    
    //Camera Detection channel
    int mCameraFX;
    //Camera Motor channel
    int mCameraMotorFX;
    
    //Sound related helpers
    inline float GetMotorVolume();
    inline void StopMotorSound();
    inline void StartMotorSound();
    inline void StartDetectSound();
    
    //Sound decay function
    inline float LinearDecay(float distance){
        return 192.0f - 0.128f * distance;
    }
};

#endif /* SecurityCamera_hpp */
