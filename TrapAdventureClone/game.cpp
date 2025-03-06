#include"game.h"
#include<iostream>

Game::Game() : window(nullptr), isRunning(false) {};

Game:: ~Game() {
	renderer.ShutDown();
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Trap Adventure Clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!renderer.init(window)) {
        std::cerr << "Lỗi khởi tạo renderer." << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    isRunning = true;
    return true;
}

void Game::run() {
    Uint32 lastTime = SDL_GetTicks();
    Uint32 currentTime;
    float deltaTime;
    
    while (isRunning) {
        currentTime = SDL_GetTicks();
        deltaTime = ( currentTime - lastTime ) / 1000.0f;
        lastTime = currentTime;

        handInput();
        update(deltaTime);
        render();
    }
}

void Game::handInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
    }
}

void Game::update(float deltaTime) {
    // 
}

void Game::render() {
    renderer.Clear();

    renderer.Present();
}
