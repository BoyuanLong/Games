//
//  HeightMap.cpp
//  Game-mac
//
//  Created by Sky X on 10/27/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "HeightMap.hpp"
#include <iostream>
#include <fstream>

using std::string;
using std::vector;

HeightMap::HeightMap(){
    std::ifstream infile("Assets/HeightMap/HeightMap.csv");
    string line;
    while(std::getline(infile, line)){
        vector<string> elements = CSVHelper::Split(line);
        vector<int> lineInt(elements.size());
        for(size_t i = 0; i < lineInt.size(); ++i){
            lineInt[i] = std::stoi(elements[i]);
        }
        heightmap.emplace_back(lineInt);
        
    }
}

bool HeightMap::IsCellOnTrack(int row, int col){
    if(row > (int)heightmap.size() || col > (int)heightmap[0].size()
       || row < 0 || col < 0){
        return false;
    }
    return heightmap[row][col] != -1;
}

float HeightMap::GetHeightFromCell(int row, int col){
    return -40.0f + heightmap[row][col] * 5.0f;
}

Vector3 HeightMap::CellToWorld(int row, int col){
    if(IsCellOnTrack(row, col)){
        int x = Origin_X - Width * row;
        int y = Origin_Y + Width * col;
        int z = GetHeightFromCell(row, col);
        return Vector3(x, y, z);
    }
    else{
        return Vector3::Zero;
    }
}

void HeightMap::WorldToCell(int x, int y, int &row, int &col){
    row = (Math::Abs(x - Origin_X) + Width / 2.0f) / Width;
    col = (Math::Abs(y - Origin_Y) + Width / 2.0f) / Width;
}

bool HeightMap::IsOnTrack(int x, int y){
    int row = 0;
    int col = 0;
    WorldToCell(x, y, row, col);
    return IsCellOnTrack(row, col);
}

int HeightMap::GetHeight(int x, int y){
    if(IsOnTrack(x, y)){
        int row = 0;
        int col = 0;
        WorldToCell(x, y, row, col);
        return GetHeightFromCell(row, col);
    }
    else{
        return -1000;
    }
}
