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
#include "Player.hpp"
#include "Spawner.hpp"
#include "SDL/SDL_mixer.h"
#include "TileBGComponent.hpp"
#include "CSVHelper.h"
#include "Collider.hpp"
#include "Door.hpp"
#include "SecretBlock.hpp"
#include "CollisionComponent.h"
#include "Key.hpp"
#include "Bat.hpp"
#include "Skeleton.hpp"
#include "Boss.hpp"
#include "Triforce.hpp"

const int WINW = 600;
const int WINH = 448;



bool Game::Initialize(){
    //Initialization window and renderer
    Random::Init();
    bool ifInit = SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
    this->window = SDL_CreateWindow("GAME", 0, 0, WINW, WINH, 0);
    this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    bool imgInit = ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == IMG_INIT_PNG);
    
    //Initialize variables
    gameOver = false;
    
    time = SDL_GetTicks();
    
    cameraPosition = Vector2(0, 0);
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    
    LoadData();
    
    winState = false;
    
    
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
    time = SDL_GetTicks();
    deltatime = deltatime > 0.033f ? 0.033f : deltatime;
    
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
    
}

void Game::GenerateOutput(){
    //Set Background Color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    
    //Sprite all visible components
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
    bgm = Mix_PlayChannel(-1, GetSound("Assets/Sounds/Music.ogg"), -1);
    Actor* a = new Actor(this);
    TileBGComponent* t = new TileBGComponent(a);
    t->LoadTileCSV("Assets/Dungeon/DungeonMapBG.csv", 32, 32);
    t->SetTexture(GetTexture("Assets/Dungeon/DungeonTiles.png"));
    
    //Load CSV files for all rooms
    for (int i = 1; i < 8; ++i) {
        std::string fileName = "Assets/Dungeon/";
        std::string roomName = "Room";
        roomName += i + '0';
        fileName += roomName + ".csv";
        RoomFirstTime[i] = false;
        
        std::ifstream infile(fileName);
        std::string line;
        while(std::getline(infile, line)){
            std::vector<std::string> elements = CSVHelper::Split(line);
            if(elements[0] == "Type") continue;
            if(elements[0] == "Player" ){
                p = new Player(this);
                p->SetPosition(Vector2(static_cast<float>(std::stoi(elements[1])) , static_cast<float>(std::stoi(elements[2])) ));
                currentRoom = roomName;
            }
            else if(elements[0] == "Collider"){
                Collider* c = new Collider(this);
                int w = std::stoi(elements[3]);
                int h = std::stoi(elements[4]);
                c->SetDimention(w, h);
                c->SetPosition(Vector2(static_cast<float>(std::stoi(elements[1]) + w/2) , static_cast<float>(std::stoi(elements[2]) + h/2) ));
                colliders.push_back(c);
            }
            else if(elements[0] == "Door"){
                Door* d = new Door(this);
                int w = std::stoi(elements[3]);
                int h = std::stoi(elements[4]);
                d->SetPosition(Vector2(static_cast<float>(std::stoi(elements[1]) + w/2), static_cast<float>(std::stoi(elements[2]) + h/2) ));
                d->SetUp(elements[7], elements[6], elements[5]);
                d->UpdateTexture();
                
                doorMap[roomName].push_back(d);
            }
            else if(elements[0] == "SecretBlock"){
                int w = std::stoi(elements[3]);
                int h = std::stoi(elements[4]);
                SecretBlock* sb = new SecretBlock(this, elements[6], Vector2(static_cast<float>(std::stoi(elements[1]) + w/2) , static_cast<float>(std::stoi(elements[2]) + h/2)));
                CollisionComponent* cc = new CollisionComponent(sb);
                cc->SetSize(static_cast<float>(w), static_cast<float>(h));
                sb->SetPosition(Vector2(static_cast<float>(std::stoi(elements[1]) + w/2) , static_cast<float>(std::stoi(elements[2]) + h/2) ));
                secrets.emplace_back(sb);
            }
            else{
                Spawner* s = new Spawner(this, elements[0]);
                int w = std::stoi(elements[3]);
                int h = std::stoi(elements[4]);
                s->SetPosition(Vector2(static_cast<float>(std::stoi(elements[1]) + w/2), static_cast<float>(std::stoi(elements[2]) + h/2) ));
                spawnerMap[roomName].emplace_back(s);
                
            }
        }
    }
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

void Game::SetCurrentRoom(std::string room){
    std::vector<Actor*> previousE = enemies[currentRoom];
    std::vector<Actor*> currentE = enemies[room];
    for(Actor* a : previousE){
        a->SetState(ActorState::Paused);
    }
    for(Actor* a : currentE){
        a->SetState(ActorState::Active);
    }
    
    
    currentRoom = room;
    int i = room[4] - '0';
    if(!RoomFirstTime[i]){
        RoomFirstTime[i] = true;
        std::vector<Spawner*> sps = spawnerMap[room];
        for(size_t i = 0; i < sps.size(); ++i){
            if(sps[i]->GetType() == "Key"){
                Key* k = new Key(this);
                k->SetPosition(sps[i]->GetPosition());
            }
            else if(sps[i]->GetType() == "Bat"){
                Bat* b = new Bat(this);
                b->SetPosition(sps[i]->GetPosition());
            }
            else if(sps[i]->GetType() == "Skeleton"){
                Skeleton* s = new Skeleton(this);
                s->SetPosition(sps[i]->GetPosition());
            }
            else if(sps[i]->GetType() == "Boss"){
                Boss* b = new Boss(this);
                b->SetPosition(sps[i]->GetPosition());
            }
            else if(sps[i]->GetType() == "Triforce"){
                Triforce* tri = new Triforce(this);
                tri->SetPosition(sps[i]->GetPosition());
            }
        }
    }

}

void Game::AddEnemy(Actor* enemy, std::string room){
    enemies[room].emplace_back(enemy);
}

void Game::RemoveEnemy(Actor* enemy, std::string room){
    auto p = std::find(enemies[room].begin(), enemies[room].end(), enemy);
    if(p != enemies[room].end())   enemies[room].erase(p);
}
