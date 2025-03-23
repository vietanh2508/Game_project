#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "game.h"
#include "level.h"

Game::Game(): window(nullptr),isRunning(false),level(nullptr),renderer(),player() {}

Game:: ~Game() {
    if(level) delete level;
	renderer.~Renderer() ;
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

    level = new Level(
        renderer,
        "assets/map/wall.csv",      
        "assets/tiles/tile",       
        "assets/map/trap.csv",      
        "assets/tiles/trap",       
        32, 32,                    
        960, 640                    
    );
    player.LoadSprites(renderer.GetSDLRenderer(), "assets/image/player_spritesheet.png");
    player.SetFrameSize(32, 32);
    player.SetDisplaySize(16, 16);

    isRunning = true;
    return true;
}

void Game::run() {
    Uint32 lastTime = SDL_GetTicks();
    Uint32 currentTime;
    float deltaTime;

    while (isRunning) {
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f; 
        lastTime = currentTime;

        handleInput();
        update(deltaTime);
        render();

        // Giới hạn FPS
        Uint32 frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < TARGET_FRAME_TIME) {
            SDL_Delay(TARGET_FRAME_TIME - frameTime); 
        }
    }
}

void Game::handleInput() {
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
    if (level) {
        const SDL_Rect playerRect = player.GetRect();
        const std::vector<Tile>& levelTiles = level->GetTiles();
        const std::vector<Trap>& levelTraps = level->GetTraps();
        std::vector<SDL_Rect> tileRects;
        for (const auto& tile : levelTiles) {
            tileRects.push_back(tile.rect);
        }
        level->Update(deltaTime, player.GetRect(), tileRects);
        player.Update(deltaTime, tileRects, levelTraps);
    }
}
void Game::render() {
    renderer.Clear();
    level->Render(renderer);
    player.Render(renderer.GetSDLRenderer());
    renderer.Present();
}
