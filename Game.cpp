#include "Game.h"

using namespace std;

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game():
    mWindow(nullptr),
    mIsRunning(true),
    mRenderer(nullptr),
    mTicksCount(0),
    mPaddleDir(0) {

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
        "Pong", // window title
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

    // hard code positions
    mPaddlePos.x = 10.0f;
    mPaddlePos.y = 768.0f/2.0f;
    mBallPos.x = 1024.0f/2.0f;
    mBallPos.y = 768.0f/2.0f;

    // hard code initial velocity of ball
    mBallVelocity.x = -200.0f;
    mBallVelocity.y = 235.0f;

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
            case SDL_QUIT: // SDL_QUIT event ends the game
                mIsRunning = false;
                break;
        }
    }

    // 'state' stores the state of keyboard
    const Uint8* state = SDL_GetKeyboardState(NULL);
    // if keyboard esc is pressed, quit loop
    if(state[SDL_SCANCODE_ESCAPE]) {
        mIsRunning = false;
    }

    // interpret paddle direction: W key for up, S key for down
    mPaddleDir = 0;
    if(state[SDL_SCANCODE_UP]) { // up
        mPaddleDir -= 1;
    }
    if(state[SDL_SCANCODE_DOWN]) { // down
        mPaddleDir += 1;
    }
}

void Game::UpdateGame() {

    // implement frame limit
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16)){
        ; // do nothing if 16ms did not pass
    }

    /* 
    SDL_GetTicks function returns the number of times passed 
    after SDL_Init function is called (in milliseconds)
    */
    /*
    deltaTime is calculated by 
    (current frame tick number) - (most recent frame tick number)
    */
    /*
    deltaTime is divided by 1000.0f to convert 
    from milliseconds to seconds
    */
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    
    /*
    prevent deltaTime from getting too big,
    by setting the maximum deltaTime    
    */
    if(deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }

    mTicksCount = SDL_GetTicks(); // update mTicksCount

    // update paddle position using mPaddleDir
    if(mPaddleDir != 0) {
        // if mPaddleDir == 0, do nothing
        mPaddlePos.y += mPaddleDir * 300.0f * deltaTime; // paddle moves 300 pixels per sec

        // if paddle moves out of frame
        if(mPaddlePos.y < (paddleH/2.0f + thickness)) {
            mPaddlePos.y = paddleH/2.0f + thickness;
        }
        else if(mPaddlePos.y > (768.0f - paddleH/2.0f - thickness)) {
            mPaddlePos.y = 768.0f - paddleH/2.0f - thickness;
        }
    }

    // update ball position using mBallVelocity
    mBallPos.x += mBallVelocity.x * deltaTime;
    mBallPos.y += mBallVelocity.y * deltaTime;


    // bouncing actions for the ball

    // 1. x-direction changes
    // check if the ball intersect with the paddle
    float distance = mPaddlePos.y - mBallPos.y;
    // get the absolute value of distance
    distance = (distance > 0.0f) ? distance : -distance;
    if(
        // if the ball is exactly above the paddle
        distance <= paddleH / 2.0f 
        // if the ball is at the left of the screen
        && (20.0f <= mBallPos.x && mBallPos.x <= 25.0f) 
        // if the ball is moving leftward
        && mBallVelocity.x < 0.0f
    ) {
        // the ball bounces to the paddle,
        // so change the x-direction of the ball
        mBallVelocity.x *= - 1.0f;
    }
    // check if the ball go off screen (leftward)
    else if(mBallPos.x <= 0.0f) {
        mIsRunning = false; // end game
    }
    // check if the ball collide with the right wall
    else if(mBallPos.x >= (1024.0f - thickness) && mBallVelocity.x > 0.0f) {
        // the ball bounces to the right wall,
        // so change the x-direction of the ball
        mBallVelocity.x *= -1.0f;
    }

    // 2. y-direction changes
    // check if the ball collide with the top wall
    if(
        mBallPos.y <= thickness 
        && mBallVelocity.y < 0.0f // if the ball is going up
    ) {
        // the ball bounces to the top wall,
        // so change the y-direction of the ball
        mBallVelocity.y *= - 1;
    }
    // check if the ball collide with the bottom wall
    else if(
        mBallPos.y >= (768 - thickness)
        && mBallVelocity.y > 0.0f // if the ball is going down
    ){
        // the ball bounces to the bottom wall,
        // so change the y-direction of the ball
        mBallVelocity.y *= -1;
    }
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
    
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

    // draw top wall
    SDL_Rect wall{
        0, // x coordinate
        0, // y coordinate
        1024, // width
        thickness // height
    };
    SDL_RenderFillRect(mRenderer, &wall);

    // draw bottom wall
    wall.y = 768 - thickness;
    SDL_RenderFillRect(mRenderer, &wall);

    // draw right wall
    wall.x = 1024 - thickness;
    wall.y = 0;
    wall.w = thickness;
    wall.h = 1024;
    SDL_RenderFillRect(mRenderer, &wall);

    // draw the paddle
    SDL_Rect paddle{
        static_cast<int>(mPaddlePos.x),
        static_cast<int>(mPaddlePos.y - paddleH/2),
        thickness,
        static_cast<int>(paddleH)
    };
    SDL_RenderFillRect(mRenderer, &paddle);

    // draw the ball
    SDL_Rect ball{
        static_cast<int>(mBallPos.x - thickness/2),
        static_cast<int>(mBallPos.y - thickness/2),
        thickness,
        thickness
    };
    SDL_RenderFillRect(mRenderer, &ball);

    SDL_RenderPresent(mRenderer); // swap front and back buffer
}