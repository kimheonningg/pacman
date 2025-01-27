#include <SDL.h>

// 2D vector
struct Vector2D {
    float x; // stores x coordinate value
    float y; // stores y coordinate value
};

class Game {
    public:
        Game(); 
        bool Initialize(); // initialize game
        void RunLoop(); // run game loop
        void Shutdown(); // end game
    private: 
        // helper functions for game loop
        void ProcessInput();
        void UpdateGame();
        void GenerateOutput();

        SDL_Window* mWindow; // window made by SDL
        bool mIsRunning; // if game is still running

        SDL_Renderer* mRenderer; // used to render 2D graphics

        Vector2D mPaddlePos; // position of paddle
        Vector2D mBallPos; // position of ball

        // number of ticks passed
        Uint32 mTicksCount;
};