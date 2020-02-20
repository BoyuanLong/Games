//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include <algorithm>
#include "Actor.h"
#include <fstream>
#include "Renderer.h"
#include "Random.h"
#include "Player.hpp"
#include "SideBlock.hpp"
#include "Block.hpp"

Game::Game()
:mIsRunning(true)
{
	
}

bool Game::Initialize()
{
	Random::Init();

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	
    mRenderer = new Renderer(this);
    if(!mRenderer->Initialize(1024.0f, 768.0f)){
        return false;
    }

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	LoadData();

	mTicksCount = SDL_GetTicks();
	
	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	for (auto actor : mActors)
	{
		actor->ProcessInput(state);
	}
}

void Game::UpdateGame()
{
	// Compute delta time
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.01667f)
	{
		deltaTime = 0.01667f;
	}
	mTicksCount = SDL_GetTicks();
	
	// Make copy of actor vector
	// (iterate over this in case new actors are created)
	std::vector<Actor*> copy = mActors;
	// Update all actors
	for (auto actor : copy)
	{
		actor->Update(deltaTime);
	}

	// Add any actors to destroy to a temp vector
	std::vector<Actor*> destroyActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == ActorState::Destroy)
		{
			destroyActors.emplace_back(actor);
		}
	}

	// Delete the destroyed actors (which will
	// remove them from mActors)
	for (auto actor : destroyActors)
	{
		delete actor;
	}
    
    if((mlastSpawn - mPlayer->GetPosition().x) < 2500){
        mlastSpawn += 500;
        SideBlock* a = new SideBlock(this, mlastSpawn / 500);
        SideBlock* b = new SideBlock(this, mlastSpawn / 500);
        a->SetPosition(Vector3(mlastSpawn, 500, 0));
        b->SetPosition(Vector3(mlastSpawn, -500, 0));
    }
    
    if((mlastBlock - mPlayer->GetPosition().x) < 2000){
        mlastBlock += 1000;
        std::string name = "Assets/Blocks/";
        if(mlastBlock/1000 <= 20){
            name += std::to_string(mlastBlock/1000);
        }
        else{
            name += std::to_string(rand() % 20 + 1);
        }
        name += ".txt";
        fileReader(name, mlastBlock);
    }
    
    if(mPlayer->GetState() == ActorState::Paused){
        Mix_HaltChannel(engine);
    }
}

void Game::GenerateOutput()
{
    mRenderer->Draw();
}

void Game::LoadData()
{
    mPlayer = new Player(this);
    Matrix4 per = Matrix4::CreatePerspectiveFOV(1.22f, 1024.0f, 768.0f, 10.0f, 10000.0f);
    mRenderer->SetProjectionMatrix(per);
    
    Matrix4 lookat = Matrix4::CreateLookAt(Vector3(-300, 0, 100), Vector3(20, 0, 0), Vector3::UnitZ);
    mRenderer->SetViewMatrix(lookat);
    
    for(int i = 0; i < 3000; i += 500){
        SideBlock* a = new SideBlock(this, i / 500);
        SideBlock* b = new SideBlock(this, i / 500);
        a->SetPosition(Vector3(i, 500, 0));
        b->SetPosition(Vector3(i, -500, 0));
        mlastSpawn = i;
    }
    
    fileReader("Assets/Blocks/1.txt", 1000);
    fileReader("Assets/Blocks/2.txt", 2000);
    mlastBlock = 2000;
    engine = Mix_PlayChannel(-1, GetSound("Assets/Sounds/ShipLoop.wav"), -1);
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	// Destroy sounds
	for (auto s : mSounds)
	{
		Mix_FreeChunk(s.second);
	}
	mSounds.clear();
}

Mix_Chunk* Game::GetSound(const std::string& fileName)
{
	Mix_Chunk* chunk = nullptr;
	auto iter = mSounds.find(fileName);
	if (iter != mSounds.end())
	{
		chunk = iter->second;
	}
	else
	{
		chunk = Mix_LoadWAV(fileName.c_str());
		if (!chunk)
		{
			SDL_Log("Failed to load sound file %s", fileName.c_str());
			return nullptr;
		}

		mSounds.emplace(fileName, chunk);
	}
	return chunk;
}

void Game::Shutdown()
{
	UnloadData();
	Mix_CloseAudio();
	// TODO: Delete renderer
    mRenderer->Shutdown();
    delete mRenderer;
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mActors.end() - 1;
		std::iter_swap(iter, iter2);
		mActors.pop_back();
	}
}


void Game::DeleteBlocK(Block* b){
    auto iter = std::find(blocks.begin(), blocks.end(), b);
    if (iter != blocks.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        auto iter2 = blocks.end() - 1;
        std::iter_swap(iter, iter2);
        blocks.pop_back();
    }
}


void Game::fileReader(std::string fileName, int dist){
    std::ifstream ifile(fileName);
    if(!ifile.is_open()){
        return;
    }
    std::string line;
    int i = 0;
    while(std::getline(ifile, line)){
        for(size_t j = 0; j < line.size(); ++j){
            if(line[j] == '.')  continue;
            else if(line[j] == 'A'){
                Block* b = new Block(this, false);
                b->SetPosition(Vector3(dist, -237.5 + 25 * j, 237.5 - 25 * i));
            }
            else{
                Block* b = new Block(this, true);
                b->SetPosition(Vector3(dist, -237.5 + 25 * j, 237.5 - 25 * i));
            }
        }
        ++i;
    }
    ifile.close();
}
