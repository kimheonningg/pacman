#include <SDL2/SDL.h>

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
};