#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>      
#include <vector>      
#include <string>      
#include "collision.h" 
#include "trap.h"     
#include "audio.h"     

class Player {
public:
    // Định nghĩa các trạng thái của player
    enum class State {
        RUNNING,
        JUMPING, 
        FALLING, 
        IDLE     
    };

    Player(Audio& audio);  // Constructor
    ~Player(); // Destructor

    void LoadSprites(SDL_Renderer* renderer, const std::string& path); // Tải spritesheet
    void Update(float deltaTime, const std::vector<SDL_Rect>& tiles, const std::vector<Trap>& traps); // Cập nhật trạng thái và vị trí của player
    void UpdateAnimation(float deltaTime); // Cập nhật animation dựa trên trạng thái
    void UpdateState(); // Cập nhật trạng thái của player
    void Render(SDL_Renderer* renderer); // Vẽ player lên màn hình

    // Các hàm để điều khiển player
    void MoveLeft();
    void MoveRight();
    void Jump();
    void Stop();

    SDL_Rect GetRect() const { return destRect; } // Lấy hình chữ nhật của player

    void SetFrameSize(int width, int height); // Đặt kích thước của một frame trong spritesheet
    void SetDisplaySize(int width, int height); // Đặt kích thước hiển thị của player
    void ResetPosition(); // Đặt lại vị trí của player về ban đầu

    bool isAlive;   // Player còn sống hay không
    int lives;      // Số mạng sống còn lại

private:
    Collision collision;  
    SDL_Texture* texture;   // Texture của player 
    SDL_Rect srcRect;       // Hình chữ nhật chỉ định frame hiện tại trong spritesheet
    SDL_Rect destRect;      // Hình chữ nhật chỉ định vị trí và kích thước của player trên màn hình
    State currentState;     // Trạng thái hiện tại của player
    int frameWidth, frameHeight; // Kích thước của một frame trong spritesheet
    int currentFrame, totalFrames; // Chỉ số của frame hiện tại và tổng số frame
    float frameTime, accumulator; // Các biến để quản lý thời gian animation
    float velocityX, velocityY; // Vận tốc của player theo trục X và Y
    bool isFlipped;     // Player đang quay mặt sang trái hay phải
    bool isOnGround;    // Player có đang chạm đất hay không
    Audio& audio;       // Tham chiếu đến đối tượng Audio để phát âm thanh
    SDL_Texture* livesTexture; // Texture cho hiển thị số mạng sống còn lại
    SDL_Rect livesSrcRect; // Hình chữ nhật chỉ định frame trong livesTexture
};

#endif