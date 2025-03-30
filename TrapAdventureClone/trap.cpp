#include "trap.h"
#include "collision.h"  // Sử dụng lớp Collision
#include <cmath>        // std::hypot (tính khoảng cách)

// Constructor: Khởi tạo bẫy
Trap::Trap(int x, int y, int w, int h, SDL_Texture* texture, Behavior behavior)
    : rect{ x, y, w, h }, texture(texture), behavior(behavior), initialX(x), initialY(y) {}

// GetRect: Lấy hình chữ nhật của bẫy
SDL_Rect Trap::GetRect() const {
    return rect;
}

// GetTexture: Lấy texture của bẫy
SDL_Texture* Trap::GetTexture() const {
    return texture;
}

// Update: Cập nhật trạng thái của bẫy
void Trap::Update(float deltaTime, const SDL_Rect& playerRect, const std::vector<SDL_Rect>& walls) {
    if (!isActivated) {
        // Tính khoảng cách giữa player và bẫy
        float dx = static_cast<float>(playerRect.x + playerRect.w / 2 - rect.x + rect.w / 2); // Khoảng cách theo trục X
        float dy = static_cast<float>(playerRect.y + playerRect.h / 2 - rect.y + rect.h / 2); // Khoảng cách theo trục Y
        float distance = std::hypot(dx, dy); // Tính khoảng cách

        // Khoảng cách kích hoạt
        if (distance <= 96.0f) {
            isActivated = true; 
        }
        return;
    }

    // Nếu bẫy đã va chạm với tường, không cập nhật nữa
    if (hasHitWall) return;

    // Dựa vào loại hành vi để cập nhật trạng thái của bẫy
    switch (behavior) {
    case Behavior::SHOOT_LEFT: {
        rect.x -= static_cast<int>(slideSpeed * deltaTime); // Di chuyển bẫy sang trái
        for (const auto& wall : walls) { // Kiểm tra va chạm với tường
            if (collision.CheckCollision(rect, wall)) { 
                hasHitWall = true; 
                break; 
            }
        }
        break;
    }
    case Behavior::SHOOT_RIGHT: { 
        rect.x += static_cast<int>(slideSpeed * deltaTime); // Di chuyển bẫy sang phải
        for (const auto& wall : walls) { // Kiểm tra va chạm với tường
            if (collision.CheckCollision(rect, wall)) { 
                hasHitWall = true;
                break; 
            }
        }
        break;
    }
    case Behavior::SLIDE_RIGHT: { 
        if (rect.x < initialX + 32) { // Nếu chưa trượt hết quãng đường
            rect.x += static_cast<int>(slideSpeed * deltaTime); // Di chuyển bẫy sang phải
            if (rect.x >= initialX + 32) { 
                rect.x = initialX + 32;  // Đặt lại vị trí chính xác
                hasHitWall = true; 
            }
        }
        break;
    }
    case Behavior::SLIDE_LEFT: { 
        if (rect.x > initialX - 32) { // Nếu chưa trượt hết quãng đường
            rect.x -= static_cast<int>(slideSpeed * deltaTime); // Di chuyển bẫy sang trái
            if (rect.x <= initialX - 32) { 
                rect.x = initialX - 32;  // Đặt lại vị trí chính xác
                hasHitWall = true; 
            }
        }
        break;
    }

    case Behavior::FALL: {
        if (!hasHitWall) { 
            rect.y += static_cast<int>(fallSpeed * deltaTime); // Di chuyển bẫy xuống dưới
            fallSpeed += gravity * deltaTime; // Tăng tốc độ rơi 

            for (const auto& wall : walls) { // Kiểm tra va chạm với tường
                if (collision.CheckCollision(rect, wall)) { 
                    hasHitWall = true; 
                    fallSpeed = 0;  
                    rect.y = wall.y - rect.h; // Đặt bẫy lên trên tường
                    break; 
                }
            }
        }
        break;
    }

    default: // Bẫy STATIC 
        break;
    }
}

//  Reset trạng thái của bẫy về ban đầu
void Trap::Reset() {
    rect.x = initialX; // Đặt lại vị trí X
    rect.y = initialY; // Đặt lại vị trí Y
    isActivated = false; // Đặt lại trạng thái kích hoạt
    hasHitWall = false; // Đặt lại trạng thái va chạm với tường
    fallSpeed = 400.0f; // Đặt lại tốc độ rơi
}