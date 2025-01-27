#include "Game.h"

using namespace std;

Game::Game():mWindow(nullptr), mIsRunning(true){

}

// true if successfully initialized
bool Game::Initialize() {
    
    // initialize SDL library
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);

    // initialization success if sdlResult == 0
    if(sdlResult != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // if successfully initialized, create window
    mWindow = SDL_CreateWindow(
        "Pacman", // window title
        100, // x coordinate
        100, // y coordinate
        1024, // window width
        768, // window height
        0 // no flag
    );

    if(!mWindow) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(
        mWindow,
        -1, // use default graphic card
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if(!mRenderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    return true;
}

void Game::Shutdown() {
    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(mRenderer);
    SDL_Quit(); // close SDL
}

void Game::RunLoop() {
    while(mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput() {
    SDL_Event event;

    // while event still exists in queue
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT: // keyboard X quits the loop
                mIsRunning = false;
                break;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    // if keyboard esc is pressed, quit loop
    if(state[SDL_SCANCODE_ESCAPE]) {
        mIsRunning = false;
    }
}

void Game::UpdateGame() {

}

void Game::GenerateOutput() {
    // draw graphics
    SDL_SetRenderDrawColor(
        mRenderer,
        0, // R
        0, // G
        255, // B
        255 // A
    );
    SDL_RenderClear(mRenderer); // delete back buffer
    SDL_RenderPresent(mRenderer); // swap front and back buffer
}