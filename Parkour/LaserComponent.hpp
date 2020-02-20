//
//  LaserComponent.hpp
//  Game-mac
//
//  Created by Sky X on 11/17/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef LaserComponent_hpp
#define LaserComponent_hpp

#include "MeshComponent.h"
#include "SegmentCast.h"

class LaserComponent : public MeshComponent{
public:
    LaserComponent(class Actor* owner);
    void Draw(class Shader* shader) override;
    void Update(float deltaTime) override;
    
private:
    Matrix4 WorldTrans(LineSegment line);
    
    //LineSegment mLine;
    std::vector<LineSegment> mLines;
};

#endif /* LaserComponent_hpp */
