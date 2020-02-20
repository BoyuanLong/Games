#pragma once

#ifndef GAME_H
#define GAME_H

#include "SDL/SDL.h"
#include <vector>
#include <unordered_map>
#include <string>
#include "SpriteComponent.h"
class Actor;
class Asteroid;

const int WINW = 1024;
const int WINH = 768;

class Game{
public:
    bool Initialize();
    void Shutdown();
    void Runloop();
    
    void AddActor(Actor* actor);
    void RemoveActor(Actor* actor);
    
    SDL_Texture* GetTexture(std::string fileName);
    
    void AddSprite(SpriteComponent* comp);
    void RemoveSprite(SpriteComponent* comp);
    
    //Asteroid functions
    void AddAsteroid(Asteroid* aster) { asteroids.push_back(aster); };
    void RemoveAsteroid(Asteroid* aster);
    const std::vector<Asteroid*>& GetAsteroid(){ return asteroids; };
    
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    Uint32 time;
    
    //Basic Loop
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    
    //Load/Unload all data
    void LoadData();
    void UnloadData();
    
    bool gameOver;
    
    //Actor list
    std::vector<Actor*> actors;
    //Sprites list
    std::vector<SpriteComponent*> spriteComponents;
    //Asteroids list
    std::vector<Asteroid*> asteroids;
    //Texture buffer
    std::unordered_map<std::string, SDL_Texture*> mapT;
};

#endif
