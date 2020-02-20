//
//  TileBGComponent.hpp
//  Game-mac
//
//  Created by Sky X on 9/26/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef TileBGComponent_hpp
#define TileBGComponent_hpp

#include "SpriteComponent.h"
#include <string>
#include <vector>
#include <algorithm>

class TileBGComponent : public SpriteComponent{
public:
    TileBGComponent(class Actor* owner, int drawOrder = 50);
    
    void Draw(SDL_Renderer* renderer) override;
    void LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight);
    
private:
    int mTileWidth;
    int mTileHeight;
    std::vector<std::vector<int> > csvTiles;
    
};

#endif /* TileBGComponent_hpp */
