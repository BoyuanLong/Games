#pragma once


#ifndef GAME_H
#define GAME_H

#include "SDL/SDL.h"
#include <vector>
#include <unordered_map>
#include <string>
#include "SpriteComponent.h"
#include "Math.h"
#include "Player.hpp"
#include "SDL/SDL_mixer.h"
class Actor;
class Block;
class Player;
class Goomba;

namespace GAME {
const float Gravity = 2000.0f;
const float MarioGravity = 2000.0f;
const float Destination = 6368.0f;
const float GoundLevel = 460.0f;
const float WINW = 600;
const float WINH = 448;
const float Sprite_Start_Position_X = 16.0f;
const float Sprite_Start_Position_Y = 16.0f;
}

class Game{
public:
    const float Gravity = 2000.0f;
    
    bool Initialize();
    void Shutdown();
    void Runloop();
    
    void AddActor(Actor* actor);
    void RemoveActor(Actor* actor);
    
    SDL_Texture* GetTexture(std::string fileName);
    
    void AddSprite(SpriteComponent* comp);
    void RemoveSprite(SpriteComponent* comp);
    
    void AddBlock(Block* block);
    void DeleteBlock(Block* block);
    
    void fileReader(std::string fileName);
    
    std::vector<Block*>& GetBlocks() { return blocks; };
    Vector2& getCamera(){ return cameraPosition; };
    void SetCameraX(float x){ cameraPosition.x = x; };
    
    void AddGoomba(Goomba* goomba){ enemies.push_back(goomba); };
    void RemoveGoomba(Goomba* goomba);
    const std::vector<Goomba*> GetGoombas() {return enemies; };
    
    const Vector2& GetMarioPosition(){ return p->GetPosition(); };
    
    Mix_Chunk* GetSound(const std::string& filename);
    
    void PlayerWin(){ winState = true; };
    bool ifWin(){ return winState; };
    
    void CameraManager(const Vector2& MarioPosition);
    
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    Uint32 time;
    
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    
    void LoadData();
    void UnloadData();
    
    bool gameOver;
    
    std::vector<Actor*> actors;
    std::vector<SpriteComponent*> spriteComponents;
    std::unordered_map<std::string, SDL_Texture*> mapT;
    std::unordered_map<std::string, Mix_Chunk*> soundMap;
    std::vector<Block*> blocks;
    std::vector<Goomba*> enemies;
    
    int bgm;
    Player* p;
    Vector2 cameraPosition;
    bool winState;
    bool once;
};

#endif
