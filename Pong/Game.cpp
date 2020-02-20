//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include <algorithm>

bool Game::Initialize(){
    //Set Initial Window size
    WINW = WIN_START_W;
    WINH = WIN_START_H;
    
    //Initialize window and renderer
    bool ifInit = SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
    this->window = SDL_CreateWindow("GAME", 0, 0, WINW, WINH, SDL_WINDOW_RESIZABLE);
    this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    //Initialize variables
    gameOver = false;
    
    time = SDL_GetTicks();
    
    //Paddle
    moveDirection = 0;
    paddlePoint.x = PAD_START_POSITION_X;
    paddlePoint.y = PAD_START_POSITION_Y;
    
    //Ball
    ballPoint.x = BALL_START_POSITION;
    ballPoint.y = BALL_START_POSITION;
    ballVelocity.x = BALL_START_VELOCITY;
    ballVelocity.y = BALL_START_VELOCITY;
    
    return window && renderer && !ifInit;
}

void Game::Runloop(){
    while(!gameOver){
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
            gameOver = true;
        }
    }
    
    //Get KB status
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    //If Escape is pressed, Quit game
    if(state[SDL_SCANCODE_ESCAPE]){
        gameOver = true;
    }
    
    //Set moveDirection
    if(state[SDL_SCANCODE_DOWN]){
        moveDirection = 1;
    }
    else if(state[SDL_SCANCODE_UP]){
        moveDirection = -1;
    }
    else{
        moveDirection = 0;
    }
}

void Game::Shutdown(){
    //Destroy all data initialized
    if(renderer){
        SDL_DestroyRenderer(renderer);
    }
    if(window){
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

void Game::UpdateGame(){
    //Get Window Size if the size changed
    SDL_GetWindowSize(window, &WINW, &WINH);
    
    //Set deltatime
    //Deltatime is bounded by [0.016, 0.033]
    while(SDL_GetTicks() - 16 < time);
    float deltatime = static_cast<float>(SDL_GetTicks() - time) / 1000;
    deltatime = deltatime > 0.033f ? 0.033f : deltatime;
    time = SDL_GetTicks();
    
    //Move paddle
    //Use Integer Correction (moveDirection == 1)
    //if Moving upward, position needs to be added 1 due to integer casting
    paddlePoint.y += static_cast<int>(deltatime * moveDirection * PADS + (moveDirection == 1));
    if(paddlePoint.y < (PADL >> 1) + RECH)  paddlePoint.y = (PADL >> 1) + RECH;
    if(paddlePoint.y > WINH - RECH - (PADL >> 1)) paddlePoint.y = WINH - RECH - (PADL >> 1);
    
    //Ball update
    ballPoint.x += static_cast<int>(ballVelocity.x * deltatime);
    ballPoint.y += static_cast<int>(ballVelocity.y * deltatime);
    
    if(ballPoint.x < RECH){
        //Rebounce when ball touches paddle
        if(ballPoint.y <= paddlePoint.y + (PADL >> 1) && paddlePoint.y - (PADL >> 1) <= ballPoint.y){
            ballPoint.x = RECH;
            ballVelocity.x = -ballVelocity.x;
        }
        //GameOver if paddle failed to catch the ball
        else{
            gameOver = true;
        }
    }
    
    //Bounces when touching boundries,
    if(ballPoint.x > WINW - (RECH << 1)){
        ballPoint.x = WINW - (RECH << 1);
        ballVelocity.x = -ballVelocity.x;
    }
    if(ballPoint.y > WINH - (RECH << 1)){
        ballPoint.y = WINH - (RECH << 1);
        ballVelocity.y = -ballVelocity.y;
    }
    if(ballPoint.y < RECH){
        ballPoint.y = RECH;
        ballVelocity.y = -ballVelocity.y;
    }
}

void Game::GenerateOutput(){
    //Set DrawColor to blue
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    //Clear back buffer to render
    SDL_RenderClear(renderer);
    
    //Set DrawColor to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    //Draw the array of the boundries (3 Recs)
    SDL_Rect boundries[3];
    boundries[0] = {0, 0, WINW, RECH};
    boundries[1] = {WINW - RECH, 0, RECH, WINH};
    boundries[2] = {0, WINH - RECH, WINW, RECH};
    SDL_RenderFillRects(renderer, boundries, 3);
    
    //Draw Paddle
    SDL_Rect paddle{paddlePoint.x, paddlePoint.y - (PADL >> 1), RECH, PADL};
    SDL_RenderFillRect(renderer, &paddle);
    
    //Draw Ball
    SDL_Rect ball{ballPoint.x, ballPoint.y, RECH, RECH};
    SDL_RenderFillRect(renderer, &ball);
    
    SDL_RenderPresent(renderer);
}
