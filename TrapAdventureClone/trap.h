#ifndef TRAP_H
#define TRAP_H

#include <SDL.h>       
#include <vector>      
#include "collision.h"

class Trap {
public:
    // Enum Behavior: Định nghĩa các loại hành vi của bẫy
    enum class Behavior {
        STATIC,         // Bẫy tĩnh 
        SLIDE_RIGHT,    // Bẫy trượt sang phải
        SLIDE_LEFT,     // Bẫy trượt sang trái
        FALL,           // Bẫy rơi
        SHOOT_LEFT,     // Bẫy bắn sang trái
        SHOOT_RIGHT     // Bẫy bắn sang phải
    };

    // Constructor: Khởi tạo bẫy
    Trap(int x, int y, int w, int h, SDL_Texture* texture, Behavior behavior);

    SDL_Rect GetRect() const;   // Lấy hình chữ nhật của bẫy
    SDL_Texture* GetTexture() const; // Lấy texture của bẫy
    void Update(float deltaTime, const SDL_Rect& playerRect, const std::vector<SDL_Rect>& walls); // Cập nhật trạng thái của bẫy
    void Reset();           // Reset trạng thái của bẫy về ban đầu

private:
    SDL_Rect rect;          // Hình chữ nhật định vị và kích thước của bẫy
    SDL_Texture* texture;       // Texture của bẫy
    Behavior behavior;        // Loại hành vi của bẫy
    Collision collision;      // Đối tượng Collision để phát hiện va chạm

    bool isActivated = false;   // Bẫy đã được kích hoạt chưa?
    bool hasHitWall = false;    // Bẫy đã va chạm với tường chưa?
    float slideSpeed = 200.0f;  // Tốc độ trượt
    float fallSpeed = 400.0f;   // Tốc độ rơi
    const float gravity = 980.0f; // Gia tốc trọng trường 
    int initialX, initialY;    // Vị trí ban đầu của bẫy
};

#endif