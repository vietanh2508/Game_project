#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "game.h"
#include "level.h"
#include "menu.h"
#include "renderer.h"
#include "player.h"

Game::Game()
    : window(nullptr),
    isRunning(false),
    level(nullptr),
    renderer(), menu(nullptr) {
    currentGameState = GameState::MENU;
}

Game:: ~Game() {
    if (level) delete level;
    if (menu) delete menu;
    SDL_DestroyWindow(window);
    IMG_Quit();
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

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "Lỗi khởi tạo SDL_image: " << IMG_GetError() << std::endl;
        return false;

    }

    if (!renderer.init(window)) {
        std::cerr << "Renderer could not be created!" << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    gameOverTexture = renderer.LoadTexture("assets/image/game_over.png");
    if (!gameOverTexture) {
        std::cerr << "Failed to load game over texture!" << std::endl;
        return false;
    }
    victoryTexture = renderer.LoadTexture("assets/image/victory.png"); 
    if (!victoryTexture) {
        std::cerr << "Failed to load victory texture!" << std::endl;
        return false;
    }
    destinationTexture = renderer.LoadTexture("assets/image/destination.png"); 
    if (!destinationTexture) {
        std::cerr << "Failed to load destination texture!" << std::endl;
        return false;
    }

    if (!audio.Init()) {
        return false;
    }

    player = new Player(audio);
    audio.LoadSound("button_click", "assets/sound/button_click.wav");
    audio.LoadSound("run", "assets/sound/run.wav");
    audio.LoadSound("jump", "assets/sound/jump.wav");
    audio.LoadSound("death", "assets/sound/death.wav");
    audio.LoadSound("victory", "assets/sound/victory.wav");
    audio.LoadSound("game_over", "assets/sound/game_over.wav");

    menu = new Menu(renderer, audio);  
    level = new Level(
        renderer,
        "assets/map/wall.csv",
        "assets/tiles/tile",
        "assets/map/trap.csv",
        "assets/tiles/trap",
        32, 32,
        960, 640
    );
    player->LoadSprites(renderer.GetSDLRenderer(), "assets/image/player_spritesheet.png");
    player->SetFrameSize(32, 32);
    player->SetDisplaySize(16, 16);
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
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        menu->HandleEvent(event);
    }

    // Xử lý trạng thái menu
    if (currentGameState == GameState::MENU) {
        Menu::Action action = menu->GetAction();
        switch (action) {
        case Menu::Action::PLAY:
            currentGameState = GameState::PLAYING;
            menu->ResetSelection();
            break;
        case Menu::Action::EXIT:
            isRunning = false;
            break;
        default:
            break;
        }
    }
    else if (currentGameState == GameState::PLAYING || currentGameState == GameState::PAUSED) {
        if (menu->GetAction() == Menu::Action::PAUSE) {
            if (menu->IsPaused()) {
                currentGameState = GameState::PAUSED;
            }
            else {
                currentGameState = GameState::PLAYING;
            }
            menu->ResetSelection();
        }
        if (menu->GetAction() == Menu::Action::EXIT) {
            isRunning = false;
        }
        const Uint8* state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_LEFT])
        {
            player->MoveLeft();
        }
        else if (state[SDL_SCANCODE_RIGHT])
        {
            player->MoveRight();
        }
        else {
            player->Stop();
        }
        if (state[SDL_SCANCODE_SPACE])
        {
            player->Jump();
        }
    }
}

void Game::update(float deltaTime) {
    switch (currentGameState) {
    case GameState::PLAYING: {
        if (level) {
            const SDL_Rect playerRect = player->GetRect();
            const std::vector<Tile>& levelTiles = level->GetTiles();
            const std::vector<Trap>& levelTraps = level->GetTraps();
            std::vector<SDL_Rect> tileRects;

            for (const auto& tile : levelTiles) {
                tileRects.push_back(tile.rect);
            }

            level->Update(deltaTime, playerRect, tileRects);
            player->Update(deltaTime, tileRects, levelTraps);

            if (playerRect.x > (30 - 1) * 32 && playerRect.y / 32 == 10 ) { 
                currentGameState = GameState::VICTORY;
                audio.PlaySound("victory");
            }

            if (player->lives < previousLives) {
                player->ResetPosition();
                level->ResetTraps();
                previousLives = player->lives;
            }
            if (!player->isAlive) {
                previousLives = player->lives;
            }
        }
        break;
    }

    case GameState::PAUSED: {

        break;
    }

    case GameState::MENU:
    case GameState::GAME_OVER:
    default:
        break;
    }
}
void Game::render() {
    renderer.Clear();

    if (currentGameState == GameState::MENU) {
        menu->Render(Menu::State::MAIN_MENU);
    }
    else if (currentGameState == GameState::PLAYING || currentGameState == GameState::PAUSED) {
        level->Render(renderer);
        player->Render(renderer.GetSDLRenderer());
        menu->Render(Menu::State::IN_GAME);

        SDL_Rect destinationRect = { (30 - 1) * 32, 10 * 32, 32, 32 }; 
        renderer.RenderTexture(destinationTexture, SDL_Rect{ 0, 0, 32, 32 }, destinationRect);

        if (!player->isAlive) {
            static bool hasPlayed = false;
            if (!hasPlayed) {
                audio.PlaySound("game_over");
                hasPlayed = true;
            }
            SDL_Rect gameOverRect = { 0, 0, 960, 640 };
            renderer.RenderTexture(gameOverTexture, SDL_Rect{ 0, 0, 960, 640 }, gameOverRect);
        }
    }
    
    else if (currentGameState == GameState::VICTORY) {
        SDL_Rect victoryRect = { 0, 0, 960, 640 };
        renderer.RenderTexture(victoryTexture, SDL_Rect{ 0, 0, 960, 640 }, victoryRect);
    }
    renderer.Present();
}