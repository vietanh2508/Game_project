#include <SDL.h>      
#include <SDL_image.h>  
#include <iostream>   
#include "game.h"       
#include "level.h"     
#include "menu.h"       
#include "renderer.h"  
#include "player.h"     

// Constructor
Game::Game()
    : window(nullptr), isRunning(false), level(nullptr), 
    renderer(), menu(nullptr), destinationTexture(nullptr), 
    gameOverTexture(nullptr), victoryTexture(nullptr) ,player(nullptr) {
    currentGameState = GameState::MENU; 
}

// Destructor
Game::~Game() {
    if (level) delete level;  
    if (menu) delete menu;     
    SDL_DestroyWindow(window); 
    IMG_Quit();            
    SDL_Quit();           
}

// Khởi tạo game
bool Game::init() {
    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) { 
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Tạo cửa sổ
    window = SDL_CreateWindow("Trap Adventure Clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 640, SDL_WINDOW_SHOWN); 
    if (window == nullptr) { 
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Khởi tạo SDL_image
    int imgFlags = IMG_INIT_PNG; 
    if (!(IMG_Init(imgFlags) & imgFlags)) { 
        std::cerr << "Lỗi khởi tạo SDL_image: " << IMG_GetError() << std::endl;
        return false;
    }

    // Khởi tạo Renderer
    if (!renderer.init(window)) { 
        std::cerr << "Renderer could not be created!" << std::endl;
        SDL_DestroyWindow(window); 
        SDL_Quit();           
        return false;
    }

    gameOverTexture = renderer.LoadTexture("assets/image/game_over.png"); // Tải texture cho màn hình game over
    if (!gameOverTexture) { 
        std::cerr << "Failed to load game over texture!" << std::endl;
        return false;
    }
    victoryTexture = renderer.LoadTexture("assets/image/victory.png"); // Tải texture cho màn hình chiến thắng
    if (!victoryTexture) { 
        std::cerr << "Failed to load victory texture!" << std::endl;
        return false;
    }
    destinationTexture = renderer.LoadTexture("assets/image/destination.png"); // Tải texture cho điểm đến
    if (!destinationTexture) { 
        std::cerr << "Failed to load destination texture!" << std::endl;
        return false;
    }

    // Khởi tạo Audio
    if (!audio.Init()) { 
        return false;
    }

    // Tạo các đối tượng khác
    player = new Player(audio); // Tạo đối tượng Player 
    audio.LoadSound("button_click", "assets/sound/button_click.wav"); // Tải âm thanh click button
    audio.LoadSound("run", "assets/sound/run.wav");          // Tải âm thanh chạy
    audio.LoadSound("jump", "assets/sound/jump.wav");        // Tải âm thanh nhảy
    audio.LoadSound("death", "assets/sound/death.wav");       // Tải âm thanh chết
    audio.LoadSound("victory", "assets/sound/victory.wav");     // Tải âm thanh chiến thắng
    audio.LoadSound("game_over", "assets/sound/game_over.wav");   // Tải âm thanh game over

    menu = new Menu(renderer, audio); // Tạo đối tượng Menu 
    level = new Level( // Tạo đối tượng Level 
        renderer,
        "assets/map/wall.csv",
        "assets/tiles/tile",
        "assets/map/trap.csv",
        "assets/tiles/trap",
        32, 32,
        960, 640
    );
    player->LoadSprites(renderer.GetSDLRenderer(), "assets/image/player_spritesheet.png"); // Tải spritesheet cho player
    player->SetFrameSize(32, 32); // Đặt kích thước frame
    player->SetDisplaySize(16, 16); // Đặt kích thước hiển thị

    isRunning = true;  // Game đã được khởi tạo thành công
    return true;
}

// Vòng lặp game chính
void Game::run() {
    Uint32 lastTime = SDL_GetTicks(); // Thời điểm của frame trước
    Uint32 currentTime;    // Thời điểm của frame hiện tại
    float deltaTime;       // Thời gian giữa hai frame

    while (isRunning) {  
        currentTime = SDL_GetTicks(); // Lấy thời điểm hiện tại
        deltaTime = (currentTime - lastTime) / 1000.0f;  
        lastTime = currentTime;  // Cập nhật thời điểm của frame trước

        handleInput(); // Xử lý input
        update(deltaTime); // Cập nhật trạng thái game
        render();      // Vẽ game lên màn hình

        // Giới hạn FPS
        Uint32 frameTime = SDL_GetTicks() - currentTime; // Thời gian thực hiện frame
        if (frameTime < TARGET_FRAME_TIME) { // Nếu frame được thực hiện quá nhanh
            SDL_Delay(TARGET_FRAME_TIME - frameTime); // Chờ để đạt được FPS mục tiêu
        }
    }
}

// Xử lý input từ người dùng
void Game::handleInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) { // Lấy tất cả các sự kiện
        if (event.type == SDL_QUIT) { 
            isRunning = false;   
        }
        menu->HandleEvent(event); // Chuyển sự kiện cho menu xử lý
    }

    // Xử lý trạng thái menu
    if (currentGameState == GameState::MENU) { 
        Menu::Action action = menu->GetAction(); // Lấy hành động mà người chơi đã chọn
        switch (action) {
        case Menu::Action::PLAY: // Nếu người chơi chọn PLAY
            currentGameState = GameState::PLAYING; 
            menu->ResetSelection();      // Reset lựa chọn của menu
            break;
        case Menu::Action::EXIT: // Nếu người chơi chọn EXIT
            isRunning = false;   // Dừng game
            break;
        default: 
            break;
        }
    }
    // Xử lý input trong game
    else if (currentGameState == GameState::PLAYING || currentGameState == GameState::PAUSED) { 
        if (menu->GetAction() == Menu::Action::PAUSE) { //Nếu người chơi chọn PAUSE
            if (menu->IsPaused()) { //Nếu menu đang tạm dừng
                currentGameState = GameState::PAUSED;
            }
            else { //Nếu menu không tạm dừng
                currentGameState = GameState::PLAYING; 
            }
            menu->ResetSelection(); // Reset lựa chọn của menu
        }
        if (menu->GetAction() == Menu::Action::EXIT) { //Nếu người chơi chọn EXIT
            isRunning = false;
        }
        const Uint8* state = SDL_GetKeyboardState(NULL); // Lấy trạng thái bàn phím
        if (state[SDL_SCANCODE_LEFT]) 
        {
            player->MoveLeft(); // Di chuyển player sang trái
        }
        else if (state[SDL_SCANCODE_RIGHT]) 
        {
            player->MoveRight(); // Di chuyển player sang phải
        }
        else { 
            player->Stop(); // Dừng di chuyển
        }
        if (state[SDL_SCANCODE_SPACE]) 
        {
            player->Jump(); // Nhảy
        }
    }
}

// Cập nhật trạng thái game
void Game::update(float deltaTime) {
    switch (currentGameState) { 
    case GameState::PLAYING: {  
        if (level) { 
            const SDL_Rect playerRect = player->GetRect(); // Lấy hình chữ nhật của player
            const std::vector<Tile>& levelTiles = level->GetTiles(); // Lấy danh sách các tile từ level
            const std::vector<Trap>& levelTraps = level->GetTraps(); // Lấy danh sách các trap từ level
            std::vector<SDL_Rect> tileRects;  // Tạo một vector để lưu trữ các hình chữ nhật của tile

            // Sao chép các hình chữ nhật của tile vào vector
            for (const auto& tile : levelTiles) {
                tileRects.push_back(tile.rect);
            }

            level->Update(deltaTime, playerRect, tileRects); // Cập nhật level 
            player->Update(deltaTime, tileRects, levelTraps);  // Cập nhật player 

            // Kiểm tra xem player đã đến đích chưa
            if (playerRect.x > (30 - 1) * 32 && playerRect.y / 32 == 10) { 
                currentGameState = GameState::VICTORY;   
                audio.PlaySound("victory");       
            }

            // Kiểm tra xem player có bị mất mạng không
            if (player->lives < previousLives) { 
                player->ResetPosition();     
                level->ResetTraps();        
                previousLives = player->lives; 
            }
            //Kiểm tra xem player còn sống hay không
            if (!player->isAlive) { 
                previousLives = player->lives; 
            }
        }
        break;
    }

    case GameState::PAUSED: { // Nếu game đang tạm dừng
        break;
    }

    case GameState::MENU: 
    case GameState::GAME_OVER: 
    default: 
        break;
    }
}

// Vẽ game lên màn hình
void Game::render() {
    renderer.Clear(); // Xóa màn hình

    if (currentGameState == GameState::MENU) { // Nếu đang ở menu
        menu->Render(Menu::State::MAIN_MENU); 
    }
    else if (currentGameState == GameState::PLAYING || currentGameState == GameState::PAUSED) { // Nếu đang chơi hoặc tạm dừng
        level->Render(renderer); // Vẽ level
        player->Render(renderer.GetSDLRenderer()); // Vẽ player
        menu->Render(Menu::State::IN_GAME); // Vẽ menu trong game

        SDL_Rect destinationRect = { (30 - 1) * 32, 10 * 32, 32, 32 }; // Vị trí của điểm đến
        renderer.RenderTexture(destinationTexture, SDL_Rect{ 0, 0, 32, 32 }, destinationRect); // Vẽ điểm đến

        // Nếu player đã chết, vẽ màn hình game over
        if (!player->isAlive) {
            static bool hasPlayed = false; //Biến static để đảm bảo âm thanh game over chỉ phát 1 lần
            if (!hasPlayed) {
                audio.PlaySound("game_over"); 
                hasPlayed = true; //Đánh dấu là đã phát âm thanh
            }
            SDL_Rect gameOverRect = { 0, 0, 960, 640 }; 
            renderer.RenderTexture(gameOverTexture, SDL_Rect{ 0, 0, 960, 640 }, gameOverRect); 
        }
    }

    else if (currentGameState == GameState::VICTORY) { // Nếu player chiến thắng
        SDL_Rect victoryRect = { 0, 0, 960, 640 };
        renderer.RenderTexture(victoryTexture, SDL_Rect{ 0, 0, 960, 640 }, victoryRect); 
    }
    renderer.Present(); // Hiển thị những gì đã vẽ
}