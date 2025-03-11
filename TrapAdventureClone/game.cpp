#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "game.h"
#include "level.h"

Game::Game(): window(nullptr),isRunning(false),level(),renderer(),player() {}

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

    window = SDL_CreateWindow("Trap Adventure Clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 640, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!renderer.init(window)) {
        std::cerr << "Renderer could not be created!" << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    player.LoadSprites(renderer.GetSDLRenderer(), "assets/image/player_spritesheet.png");
    player.SetFrameSize(32, 32);
    player.SetDisplaySize(64, 64);

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
    const Uint8* keystates = SDL_GetKeyboardState(nullptr);

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) isRunning = false;
    }

    if (keystates[SDL_SCANCODE_LEFT]) {
        player.MoveLeft();
    }
    else if (keystates[SDL_SCANCODE_RIGHT]) {
        player.MoveRight();
    }
    else {
        player.Stop();
    }

    if (keystates[SDL_SCANCODE_SPACE]) {
        player.Jump();
    }
}

void Game::update(float deltaTime) {
    player.Update(deltaTime, level.GetTilesAsRects());
}

void Game::render() {
    renderer.Clear();
    player.Render(renderer.GetSDLRenderer());
    renderer.Present();
}
