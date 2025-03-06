#include"game.h"
#include<iostream>

Game::Game() : window(nullptr), renderer(nullptr), isRunning(false);

Game:: ~Game() {
	if (renderer) delete renderer;
	if (window) SDL_DestroyWindow(window);
	SDL_Quit();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Trap Adventure 2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = new Renderer(window);
    if (!renderer->init()) {
        std::cerr << "Renderer could not be created!" << std::endl;
        return false;
    }

    isRunning = true;
    return true;
}

void Game::run() {
    Uint32 lastTime = SDL_GetTicks();
    Uint32 currentTime;
    float deltaTime;
    
    while(is)
}