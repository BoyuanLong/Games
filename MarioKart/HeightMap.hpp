//
//  HeightMap.hpp
//  Game-mac
//
//  Created by Sky X on 10/27/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#ifndef HeightMap_hpp
#define HeightMap_hpp

#include "CSVHelper.h"
#include "Math.h"

class HeightMap{
public:
    HeightMap();
    
    Vector3 CellToWorld(int row, int col);
    
    void WorldToCell(int x, int y, int& row, int& col);
    
    bool IsOnTrack(int x, int y);
    
    int GetHeight(int x, int y);
    
private:
    bool IsCellOnTrack(int row, int col);
    
    float GetHeightFromCell(int row, int col);
    
    std::vector<std::vector<int> > heightmap;
    
    const float Width = 40.55f;
    const float Origin_X = 1280.0f;
    const float Origin_Y = -1641.0f;
};

#endif /* HeightMap_hpp */
