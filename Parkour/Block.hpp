//
//  Block.hpp
//  Game-mac
//
//  Created by Sky X on 11/6/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef Block_hpp
#define Block_hpp

#include "Actor.h"

class Block : public Actor{
public:
    const float BlockScale = 64.0f;
    const Vector3 BlockSize = Vector3(1.0f, 1.0f, 1.0f);
public:
    Block(class Game* game, Actor* parent, bool mirror = false);
    
    ~Block();
    
    void SetMirror(bool m){ isMirror = m; }
    bool IsMirror(){ return isMirror; }
    
    void SetRotate(bool r){ isRotate = r; }
    bool IsRotate(){ return isRotate; }
    
    void OnUpdate(float deltaTime) override;
    
private:
    bool isMirror;
    bool isRotate;
};

#endif /* Block_hpp */
