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
#include <fstream>
#include <SDL/SDL_image.h>
#include "Random.h"
#include "Block.hpp"
#include "Player.hpp"
#include "Spawner.hpp"
#include "SDL/SDL_mixer.h"
#include "Goomba.hpp"




bool Game::Initialize(){
    //Initialization window and renderer
    Random::Init();
    bool ifInit = SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
    this->window = SDL_CreateWindow("GAME", 0, 0, GAME::WINW, GAME::WINH, 0);
    this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    bool imgInit = ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == IMG_INIT_PNG);
    
    //Initialize variables
    gameOver = false;
    
    time = SDL_GetTicks();
    
    cameraPosition = Vector2(0, 0);
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    
    LoadData();
    
    winState = false;
    once = false;
    
    return window && renderer && !ifInit && imgInit;
}

void Game::Shutdown(){
    UnloadData();
    Mix_CloseAudio();
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
    
    if(state[SDL_SCANCODE_ESCAPE])  gameOver = true;
    
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
    
    //Update actors
    std::vector<Actor*> acts = actors;
    for(Actor* a : acts){
        a->Update(deltatime);
    }
    std::vector<Actor*> toBeDestroyed;
    for(Actor* a : acts){
        if(a->GetState() == ActorState::Destroy){
            toBeDestroyed.push_back(a);
        }
    }
    for(Actor* a : toBeDestroyed){
        delete a;
    }
    
    if(p->GetState() == ActorState::Paused && !winState && !once){
        Mix_HaltChannel(bgm);
        Mix_PlayChannel(1, GetSound("Assets/Sounds/Dead.wav"), 0);
        once = true;
    }
    if(winState){
        Mix_HaltChannel(bgm);
    }
}

void Game::GenerateOutput(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    //Clear Back Buffer
    SDL_RenderClear(renderer);
    
    for(SpriteComponent* sprite : spriteComponents){
        if(sprite->IsVisible()){
            sprite->Draw(renderer);
        }
    }
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
    //Generate background
    Actor* background = new Actor(this);
    background->SetPosition(Vector2(3392, 224));
    SpriteComponent* ssc = new SpriteComponent(background, 50);
    ssc->SetTexture(GetTexture("Assets/Background.png"));
    
    //Read Level File
    fileReader("Assets/Level1.txt");
    
    //Load BGM
    bgm = Mix_PlayChannel(-1, GetSound("Assets/Sounds/Music.ogg"), -1);
}

void Game::fileReader(std::string fileName){
    std::ifstream ifile(fileName);
    if(!ifile.is_open())    return;
    std::string line;
    int i = 0;
    while(std::getline(ifile, line)){
        for(size_t j = 0; j < line.size(); ++j){
            if(line[j] == '.')  continue;
            else if(line[j] == 'P'){
                p = new Player(this);
                p->SetPosition(Vector2(PLAYER::MarioSizeX * j + GAME::Sprite_Start_Position_X,
                                       GAME::Sprite_Start_Position_Y + PLAYER::MarioSizeY * i));
                continue;
            }
            else if(line[j] == 'Y'){
                Spawner* temp = new Spawner(this);
                temp->SetPosition(Vector2(GOOMBA::GoombaSizeX * j + GAME::Sprite_Start_Position_X,
                                          GAME::Sprite_Start_Position_Y + GOOMBA::GoombaSizeY * i));
                continue;
            }
            else{
                Block* brick = new Block(this);
                brick->SetPosition(Vector2(BLOCK::BlockSizeX * j + GAME::Sprite_Start_Position_X,
                                           GAME::Sprite_Start_Position_Y + BLOCK::BlockSizeY * i));
                SpriteComponent* brick_sc = new SpriteComponent(brick, 100);
                std::string brickName = "Assets/Block";
                brickName.push_back(line[j]);
                brickName += ".png";
                brick_sc->SetTexture(GetTexture(brickName));
            }
        }
        ++i;
    }
    ifile.close();
}

void Game::UnloadData(){
    while(!actors.empty()){
        delete actors.back();
    }
    
    for(auto it = mapT.begin(); it != mapT.end(); ++it){
        SDL_DestroyTexture(it->second);
    }
    mapT.clear();
    
    for (auto it = soundMap.begin(); it != soundMap.end(); ++it) {
        Mix_FreeChunk(it->second);
    }
    soundMap.clear();
}

SDL_Texture* Game::GetTexture(std::string fileName){
    //if in buffer
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
    if(p != spriteComponents.end())   spriteComponents.erase(p);
}

void Game::AddBlock(Block *block){
    blocks.push_back(block);
}

void Game::DeleteBlock(Block *block){
    auto p = std::find(blocks.begin(), blocks.end(), block);
    if(p != blocks.end())   blocks.erase(p);
}

void Game::RemoveGoomba(Goomba *goomba){
    auto p = std::find(enemies.begin(), enemies.end(), goomba);
    if(p != enemies.end())  enemies.erase(p);
}

Mix_Chunk* Game::GetSound(const std::string& filename){
    if(soundMap.find(filename) != soundMap.end()){
        return soundMap[filename];
    }
    
    Mix_Chunk* sound = Mix_LoadWAV(filename.c_str());
    if(sound){
        if(sound) soundMap[filename] = sound;
        return sound;
    }
    return nullptr;
}

void Game::CameraManager(const Vector2 &MarioPosition){
    //Camera should follow Mario's footsetp but never rewind
    SetCameraX(std::max(std::max(MarioPosition.x - GAME::WINW / 2, 0.0f), cameraPosition.x));
}
