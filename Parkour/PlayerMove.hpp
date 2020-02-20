//
//  PlayerMove.hpp
//  Game-mac
//
//  Created by Sky X on 10/20/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef PlayerMove_hpp
#define PlayerMove_hpp

#include "MoveComponent.h"
#include "Math.h"
#include "CollisionComponent.h"

class PlayerMove : public MoveComponent{
public:
    enum MoveState{
        OnGround,
        Jump,
        Falling,
        WallClimb,
        WallRun
    };
    
    PlayerMove(class Actor* owner);
    ~PlayerMove();
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    
    void ChangeState(MoveState state){ mCurrentState = state; }
    
    void PhysicsUpdate(float deltaTime);
    void AddForce(const Vector3& force);
    void FixXYVelocity();
    
protected:
    void UpdateOnGround(float deltaTime);
    void UpdateJump(float deltaTime);
    void UpdateFalling(float deltaTime);
    void UpdateWallClimb(float deltaTime);
    void UpdateWallRun(float deltaTime);
    
    bool CanWallClimb(CollSide side, Vector3& offset);
    bool CanWallRun(CollSide side, Vector3& offset);
    CollSide FixCollision(class CollisionComponent* self, class CollisionComponent* block, Vector3& off);
    
private:
    MoveState mCurrentState;
    bool prevSpace;
    Vector3 mVelocity;
    Vector3 mAcceleration;
    Vector3 mPendingForces;
    Vector3 mWallClimb = Vector3(0.0f, 0.0f, 1800.0f);
    Vector3 mWallRunForce = Vector3(0.0f, 0.0f, 1200.0f);
    float mWallRunTimer = 0;
    float mMass;
    Vector3 mGravity;
    Vector3 mJumpForce;
    float mWallClimbTimer;
    const float MaxSpeed = 400.0f;
    const float Brake = 0.9f;
    
    int mRunningSFX;
};

#endif /* PlayerMove_hpp */
