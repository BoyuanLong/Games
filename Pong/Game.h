#pragma once

#ifndef GAME_H
#define GAME_H

#include "SDL/SDL.h"

const int RECH = 10;
const int PADL = 80;    //Paddle Length
const int PADS = 600;   //Paddle Speed
const int PAD_START_POSITION_X = 0;
const int PAD_START_POSITION_Y = 400;
const int BALL_START_POSITION = 400;
const int BALL_START_VELOCITY = 400;

const int WIN_START_W = 1024;
const int WIN_START_H = 768;

class Game{
public:
    //Game Lifetime
    bool Initialize();
    void Runloop();
    void Shutdown();
    
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    
    bool gameOver;
    
    Uint32 time;
    int moveDirection;
    
    SDL_Point paddlePoint;
    SDL_Point ballPoint;
    
    SDL_Point ballVelocity;
    
    int WINH;
    int WINW;
};

#endif
