#pragma once
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"

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

	class Renderer* GetRenderer() {	return mRenderer; }
    
    class Player* GetPlayer(){ return mPlayer; }
    
    void AddBlock(Block* b){ blocks.emplace_back(b); }
    void DeleteBlocK(Block* b);
    std::vector<Block*> GetBlock(){return blocks; }
    
    void fileReader(std::string filename, int dist);
    
    class HeightMap* GetHeightMap(){ return heightmap; }
    
    class Enemy* GetEnemy(){ return mEnemy; }
    
    int& GetBgm(){ return bgm; }
    
private:
    std::vector<Block*> blocks;
    
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
    
    class HeightMap* heightmap;
    
    class Enemy* mEnemy;
    
    float countDown;
    bool countActive;
    
    int bgm;
};
