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
#include "Door.hpp"
class Actor;
class Player;
class Collider;
class SecretBlock;
class Spawner;
class Boss;

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
    
    void fileReader(std::string fileName);
    
    Vector2& getCamera(){ return cameraPosition; };
    void SetCamera(const Vector2& pos){ cameraPosition = pos; };
    
    const Vector2& GetMarioPosition(){ return p->GetPosition(); };
    
    Mix_Chunk* GetSound(const std::string& filename);
    
    void PlayerWin(){ winState = true; };
    bool ifWin(){ return winState; };
    
    std::vector<Collider*> GetColliders(){ return colliders; };
    
    std::vector<Door*> GetDoors(std::string room){ return doorMap[room]; };
    std::string GetCurrentRoom(){ return currentRoom; };
    void SetCurrentRoom(std::string room);
    
    void AddEnemy(Actor* enemy, std::string room);
    void RemoveEnemy(Actor* enemy, std::string room);
    std::vector<Actor*> GetEnemies(){ return enemies[currentRoom]; };
    
    std::vector<SecretBlock*> getSecrets(){ return secrets; };
    
    int GetBGM(){return bgm; };
    
    Player* GetPlayer(){ return p; };
    Boss* GetBoss(){ return b; };
    
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
    
    std::vector<Collider*> colliders;
    std::unordered_map<std::string, std::vector<Door*>> doorMap;
    std::unordered_map<std::string, std::vector<Spawner*>> spawnerMap;
    std::unordered_map<std::string, std::vector<Actor*>> enemies;
    std::string currentRoom;
    int bgm;
    Player* p;
    Boss* b;
    
    bool RoomFirstTime[8];
    std::vector<SecretBlock*> secrets;
    
    //Anchor at Top Left
    Vector2 cameraPosition;
    bool winState;
};

#endif
