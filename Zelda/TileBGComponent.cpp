//
//  TileBGComponent.cpp
//  Game-mac
//
//  Created by Sky X on 9/26/19.
//  Copyright © 2019 Sanjay Madhav. All rights reserved.
//

#include "TileBGComponent.hpp"
#include "Actor.h"
#include "SpriteComponent.h"
#include "Game.h"
#include <iostream>
#include <fstream>
#include "CSVHelper.h"


TileBGComponent::TileBGComponent(Actor* owner, int drawOrder)
: SpriteComponent(owner, drawOrder)
{
    
}

void TileBGComponent::Draw(SDL_Renderer* renderer){
    for(int i = 0; i < (int)csvTiles.size(); ++i){
        for(int j = 0; j < (int)csvTiles[0].size(); ++j){
            int num = GetTexWidth() / mTileWidth;
            int y = csvTiles[i][j] / num;
            int x = csvTiles[i][j] % num;
            
            SDL_Rect src;
            src.x = x * mTileWidth;
            src.y = y * mTileHeight;
            src.w = mTileWidth;
            src.h = mTileHeight;
            
            SDL_Rect dist;
            Vector2 camera = mOwner->GetGame()->getCamera();
            dist.x = j * mTileWidth - static_cast<int>(camera.x) ;
            dist.y = i * mTileHeight - static_cast<int>(camera.y) ;
            dist.w = mTileWidth;
            dist.h = mTileHeight;
            
            
            // Draw (have to convert angle from radians to degrees, and clockwise to counter)
            SDL_RenderCopyEx(renderer,
                    mTexture,
                    &src,
                    &dist,
                    0,
                    nullptr,
                    SDL_FLIP_NONE);
        }
    }
}

void TileBGComponent::LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight)
{
    mTileWidth = tileWidth;
    mTileHeight = tileHeight;
    
    //Parse csv file into vector<vector<int>>
    std::ifstream infile(fileName);
    std::string line;
    while(std::getline(infile, line)){
        if(line.empty())    break;
        std::vector<std::string> temp = CSVHelper::Split(line);
        std::vector<int> row(temp.size());
        std::transform(temp.begin(), temp.end(), row.begin(), [](std::string s){
            return std::stoi(s);
        });
        csvTiles.push_back(row);
    }
    
    
}
