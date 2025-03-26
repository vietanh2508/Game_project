#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_mixer.h>
#include "renderer.h"
#include "player.h"
#include "level.h"
#include "menu.h"
#include "audio.h"

class Game {
public:
    Game();
    ~Game();

    enum class GameState {
        MENU,
        PLAYING,
        PAUSED,
        GAME_OVER,
        VICTORY
    };

    bool init();
    void run();
    void handleInput();
    void update(float deltaTime);
    void render();
private:
    Level* level;
    SDL_Window* window;
    Renderer renderer;
    Menu* menu;
    GameState currentGameState;
    Player* player = nullptr;
    Audio audio;
    SDL_Texture* gameOverTexture;
    SDL_Texture* victoryTexture; 
    SDL_Texture* destinationTexture;
    const float TARGET_FPS = 60.0f;
    const Uint32 TARGET_FRAME_TIME = 1000 / 60;
    bool isRunning;
    int previousLives = 10;
};

#endif