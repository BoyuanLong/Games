//
//  utils.hpp
//  Game-mac
//
//  Created by Sky X on 12/8/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef utils_hpp
#define utils_hpp


#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include "Math.h"

inline void HaltChannel(int channel){
    if(channel != -1){
        Mix_HaltChannel(channel);
    }
}

inline Quaternion CalculateQua(const Vector3& from, const Vector3& to){
    Quaternion mQua;
    //Get Normalized vector from player to the next checkpoint
    Vector3 direction = Vector3::Normalize(to - from);
    //Calculate the dot product between world front and direction
    float dot = Vector3::Dot(Vector3::UnitX, direction);
    
    
    if(dot != 1){
        if(dot == -1){
            //If they are parallel but facing the opposite
            mQua = Quaternion(Vector3::UnitZ, Math::Pi);
        }
        else{
            //If they are not parallel
            Vector3 axis = Vector3::Normalize(Vector3::Cross(Vector3::UnitX, direction));
            float angle = Math::Acos(dot);
            mQua = Quaternion(axis, angle);
        }
    }
    else{
        //If they are parallel and facing the same direction
        mQua = Quaternion(Vector3::UnitZ, 0);
    }
    return mQua;
}

inline float CalculateAngle(const Vector3& from, const Vector3& to){
    int sign = Vector3::Cross(from, to).z >= 0.0f ? 1 : -1;
    return sign * Math::Acos(Vector3::Dot(from, to) / from.Length() / to.Length());
}

//#define Set_Mesh(x) \
//    (new MeshComponent(this))->SetMesh(mGame->GetRenderer()->GetMesh(x));

#endif /* utils_hpp */
