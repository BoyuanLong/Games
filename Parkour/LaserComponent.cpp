//
//  LaserComponent.cpp
//  Game-mac
//
//  Created by Sky X on 11/17/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "LaserComponent.hpp"
#include "Actor.h"
#include "Shader.h"
#include "Mesh.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Block.hpp"
#include "Player.hpp"


LaserComponent::LaserComponent(Actor* owner)
: MeshComponent(owner, true)
{
    SetMesh(owner->GetGame()->GetRenderer()->GetMesh("Assets/Laser.gpmesh"));
}

void LaserComponent::Draw(Shader* shader){
    if (mMesh)
    {
        // Set the world transform
        
        // Set the active texture
        Texture* t = mMesh->GetTexture(mTextureIndex);
        if (t)
        {
            t->SetActive();
        }
        // Set the mesh's vertex array as active
        VertexArray* va = mMesh->GetVertexArray();
        va->SetActive();
        // Draw
        for(auto it = mLines.begin(); it != mLines.end(); ++it){
            shader->SetMatrixUniform("uWorldTransform", WorldTrans(*it));
            glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
        }
        
    }
}

void LaserComponent::Update(float deltaTime){
    mLines.clear();
    LineSegment mLine;
    mLine.mStart = mOwner->GetWorldPosition();
    mLine.mEnd = mLine.mStart + mOwner->GetWorldForward() * 500.0f;
    
    
    Actor* ignoredActor = mOwner->GetParent();
    
    //SegmentCast
    CastInfo info;
    if(SegmentCast((Actor*)mOwner->GetGame()->GetPlayer(), mLine, info)){
        mLine.mEnd = info.mPoint;
    }
//    else if(SegmentCast(mOwner->GetGame()->GetBlocks(), mLine, info)){
//        mLine.mEnd = info.mPoint;
//    }
    
    
    
    
    while(SegmentCast(mOwner->GetGame()->GetBlocks(), mLine, info, ignoredActor)){
        if(static_cast<Block*>(info.mActor)->IsMirror()){
            LineSegment line;
            line.mStart = info.mPoint;
            Vector3 coming = mLine.mEnd - mLine.mStart;
            coming.Normalize();
            line.mEnd = line.mStart + Vector3::Reflect(coming, info.mNormal) * 500.0f;
            mLine.mEnd = info.mPoint;
            mLines.emplace_back(mLine);
            mLine = line;
            ignoredActor = info.mActor;
        }
        else{
            mLine.mEnd = info.mPoint;
            mLines.emplace_back(mLine);
            break;
        }

    }
    mLines.emplace_back(mLine);
    
    CastInfo dummy;
    Player* p = mOwner->GetGame()->GetPlayer();
    for(auto line : mLines){
        if(SegmentCast((Actor*)p, line, dummy)){
            Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Laser.wav"), 0);
            p->Die();
        }
    }
}

Matrix4 LaserComponent::WorldTrans(LineSegment line){
    
    Matrix4 scale = Matrix4::CreateScale(line.Length(), 1.0f, 1.0f);
    
    //Which rotatation to use?
//    Matrix4 rotation = Matrix4::CreateRotationZ(mOwner->GetRotation());
    
    Quaternion q = Quaternion::Identity;
    Vector3 v = line.mEnd - line.mStart;
    v.Normalize();
    float dot = Vector3::Dot(v, Vector3::UnitX);
    if(dot != 1){
        if(dot == -1){
            q = Quaternion(Vector3::UnitZ, Math::Pi);
        }
        else{
            Vector3 axis = Vector3::Cross(Vector3::UnitX, v);
            axis.Normalize();
            float angle = Math::Acos(dot);
            q = Quaternion(axis, angle);
        }
    }
    
    Vector3 mid = line.mStart + line.mEnd;
    mid.x /= 2;
    mid.y /= 2;
    mid.z /= 2;
    Matrix4 translation = Matrix4::CreateTranslation(mid);
    
    return scale * Matrix4::CreateFromQuaternion(q) * translation;
}
