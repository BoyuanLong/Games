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
#include "MeshComponent.h"
#include "CameraComponent.hpp"
#include "LevelLoader.h"
#include "Checkpoint.hpp"
#include "Arrow.hpp"

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
    Mix_AllocateChannels(32);
    Mix_GroupChannels(22, 31, 1);

    LoadData();

    mTicksCount = SDL_GetTicks();
    
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_GetRelativeMouseState(nullptr, nullptr);
    
    return true;
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
        if(mNextLevel != ""){
            LoadNextLevel();
        }
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
    
}

void Game::GenerateOutput()
{
    mRenderer->Draw();
}

void Game::LoadData()
{
    Matrix4 per = Matrix4::CreatePerspectiveFOV(1.22f, 1024.0f, 768.0f, 10.0f, 10000.0f);
    mRenderer->SetProjectionMatrix(per);
    
    LevelLoader::Load(this, "Assets/Tutorial.json");
    new Arrow(this);
    Mix_PlayChannel(-1, GetSound("Assets/Sounds/Music.ogg"), -1);
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


void Game::RemoveBlock(Actor *block){
    auto iter = std::find(mBlocks.begin(), mBlocks.end(), block);
    if (iter != mBlocks.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        auto iter2 = mBlocks.end() - 1;
        std::iter_swap(iter, iter2);
        mBlocks.pop_back();
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
        }
        ++i;
    }
    ifile.close();
}

void Game::AddCheck(Checkpoint *cp){
    if(mChecks.empty()){
        cp->Activate(true);
    }
    else{
        cp->Activate(false);
    }
    mChecks.push(cp);
}

Checkpoint* Game::RemoveCheck(){
    if(!mChecks.empty()){
        Checkpoint* front = mChecks.front();
        front->Activate(false);
        mChecks.pop();
        if(!mChecks.empty())
            mChecks.front()->Activate(true);
        return front;
    }
    return nullptr;
}

void Game::LoadNextLevel(){
    while(!mActors.empty()){
        delete mActors.back();
    }
    std::queue<Checkpoint*> temp;
    std::swap(temp, mChecks);
    LevelLoader::Load(this, mNextLevel);
    new Arrow(this);
    mNextLevel.clear();
}
