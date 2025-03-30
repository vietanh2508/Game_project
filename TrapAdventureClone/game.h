#ifndef GAME_H
#define GAME_H

#include <SDL.h>     
#include "renderer.h" 
#include "player.h"    
#include "level.h"     
#include "menu.h"      
#include "audio.h"     

class Game {
public:
    // Constructor
    Game();
    ~Game(); // Destructor

    // Định nghĩa các trạng thái của game
    enum class GameState {
        MENU,      
        PLAYING,  
        PAUSED,   
        GAME_OVER, 
        VICTORY    
    };

    bool init();       // Khởi tạo game 
    void run();        // Vòng lặp game chính
    void handleInput(); // Xử lý input từ người dùng
    void update(float deltaTime); // Cập nhật trạng thái game
    void render();       // Vẽ game lên màn hình

private:
    Level* level;       // Đối tượng Level để quản lý màn chơi
    SDL_Window* window;   // Cửa sổ game
    Renderer renderer;    // Đối tượng Renderer để vẽ
    Menu* menu;         // Đối tượng Menu để quản lý menu
    GameState currentGameState; // Trạng thái hiện tại của game
    Player* player;       // Đối tượng Player để quản lý nhân vật
    Audio audio;        // Đối tượng Audio để quản lý âm thanh
    SDL_Texture* gameOverTexture; // Texture cho màn hình game over
    SDL_Texture* victoryTexture;  // Texture cho màn hình chiến thắng
    SDL_Texture* destinationTexture; // Texture cho điểm đến
    const float TARGET_FPS = 60.0f;  // Số khung hình trên giây mục tiêu
    const Uint32 TARGET_FRAME_TIME = 1000 / 60; // Thời gian cho mỗi khung hình 
    bool isRunning;      // Game đang chạy hay không
    int previousLives = 10; // Số mạng sống trước đó 
};

#endif