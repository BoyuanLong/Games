#pragma once
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include <queue>

#include "utils.hpp"

class Block;

class Game
{
public:
    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();

    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);

    Mix_Chunk* GetSound(const std::string& fileName);

    class Renderer* GetRenderer() {    return mRenderer; }
    
    class Player* GetPlayer(){ return mPlayer; }
    void SetPlayer(class Player* player) { mPlayer = player; }
    
    
    void fileReader(std::string filename, int dist);
    
    
    int& GetBgm(){ return bgm; }
    
    void AddBlock(class Actor* block){ mBlocks.emplace_back(block); }
    void RemoveBlock(class Actor* block);
    std::vector<class Actor*> GetBlocks(){return mBlocks; }
    
    void AddCheck(class Checkpoint* cp);
    class Checkpoint* RemoveCheck();
    class Checkpoint* FrontCheck(){ return mChecks.empty() ? nullptr : mChecks.front(); }
    
    void SetString(std::string str){ mNextLevel = str; }
    
    void LoadNextLevel();
private:
    
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    void LoadData();
    void UnloadData();

    std::unordered_map<std::string, Mix_Chunk*> mSounds;

    // All the actors in the game
    std::vector<class Actor*> mActors;
    
    int mlastSpawn;
    int mlastBlock;

    class Renderer* mRenderer;
    class Player* mPlayer;

    Uint32 mTicksCount;
    bool mIsRunning;
    
    std::vector<class Actor*> mBlocks;
    
    std::queue<class Checkpoint*> mChecks;
    
    int bgm;
    
    std::string mNextLevel;
};
