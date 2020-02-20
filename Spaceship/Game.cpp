//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "Actor.h"
#include <algorithm>
#include <vector>
#include <SDL/SDL_image.h>
#include "Ship.hpp"
#include "Random.h"
#include "Asteroid.hpp"



bool Game::Initialize(){
    //Initialization window and renderer
    bool ifInit = SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
    this->window = SDL_CreateWindow("GAME", 0, 0, WINW, WINH, 0);
    this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    bool imgInit = ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == IMG_INIT_PNG);
    
    //Initialize variables
    gameOver = false;
    time = SDL_GetTicks();
    
    //Initialize random generator
    Random::Init();
    
    LoadData();
    
    return window && renderer && !ifInit && imgInit;
}

void Game::Shutdown(){
    UnloadData();
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
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
        if(event.type == SDL_QUIT)   gameOver = true;
    }
    
    //Get KB status
    const Uint8* state = SDL_GetKeyboardState(nullptr);
    
    if(state[SDL_SCANCODE_ESCAPE]){
        gameOver = true;
    }
    
    //Actors ProcessInput
    std::vector<Actor*> tempActor = actors;
    for(Actor* a : tempActor){
        a->ProcessInput(state);
    }
}



void Game::UpdateGame(){
    //Set deltatime
    while(SDL_GetTicks() - 16 < time);
    float deltatime = static_cast<float>(SDL_GetTicks() - time) / 1000.0f;
    deltatime = deltatime > 0.033f ? 0.033f : deltatime;
    time = SDL_GetTicks();
    
    //Update for all Actors
    std::vector<Actor*> acts = actors;
    for(Actor* a : acts){
        a->Update(deltatime);
    }
    
    //Destroy all Inactive Actors
    std::vector<Actor*> toBeDestroyed;
    for(Actor* a : acts){
        if(a->GetState() == ActorState::Destroy){
            toBeDestroyed.push_back(a);
        }
    }
    for(Actor* a : toBeDestroyed){
        delete a;
    }
}

void Game::GenerateOutput(){
    //Clear back buffer
    SDL_RenderClear(renderer);
    
    //Draw all visible Sprite
    for(SpriteComponent* sprite : spriteComponents){
        if(sprite->IsVisible()){
            sprite->Draw(renderer);
        }
    }
    
    //Present the buffer
    SDL_RenderPresent(renderer);
}

void Game::AddActor(Actor* actor){
    actors.push_back(actor);
}

void Game::RemoveActor(Actor* actor){
    auto p = std::find(actors.begin(), actors.end(), actor);
    if(p != actors.end()){
        actors.erase(p);
    }
}

void Game::LoadData(){
    //Load background
    Actor* backgrond = new Actor(this);
    backgrond->SetPosition(Vector2(512, 384));
    SpriteComponent* ssc = new SpriteComponent(backgrond, 50);
    ssc->SetTexture(GetTexture("Assets/Stars.png"));
    
    //Generate Asteroids
    for (int i = 0; i < 10; i++) {
        new Asteroid(this);
    }
    
    //Construct the Player Ship
    Ship* ship = new Ship(this);
    ship->SetPosition(Vector2(200, 200));
    ship->SetRotation(Math::PiOver2);
}

void Game::UnloadData(){
    while(!actors.empty()){
        delete actors.back();
    }
    
    for(auto it = mapT.begin(); it != mapT.end(); ++it){
        SDL_DestroyTexture(it->second);
    }
    mapT.clear();
}

SDL_Texture* Game::GetTexture(std::string fileName){
    //If in buffer
    if(mapT.find(fileName) != mapT.end()){
        return mapT[fileName];
    }
    
    SDL_Surface* image = IMG_Load(fileName.c_str());
    if(image){
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
        SDL_FreeSurface(image);
        if(texture) mapT[fileName] = texture;
        return texture;
    }
    return nullptr;
}

void Game::AddSprite(SpriteComponent* comp){
    spriteComponents.push_back(comp);
    std::sort(spriteComponents.begin(), spriteComponents.end(), [](SpriteComponent* a, SpriteComponent* b) {
        return a->GetDrawOrder() < b->GetDrawOrder();
    });
}

void Game::RemoveSprite(SpriteComponent* comp){
    auto p = std::find(spriteComponents.begin(), spriteComponents.end(), comp);
    if(p != spriteComponents.end()){
        spriteComponents.erase(p);
    }
}

void Game::RemoveAsteroid(Asteroid* aster){
    auto p = std::find(asteroids.begin(), asteroids.end(), aster);
    if(p != asteroids.end()){
        asteroids.erase(p);
    }
}
    
