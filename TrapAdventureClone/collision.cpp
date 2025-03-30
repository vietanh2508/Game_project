#include "collision.h"
#include <algorithm>
#include <iostream> 

bool Collision::HandleCollisions(SDL_Rect& playerRect, float& velocityX, float& velocityY, bool& isOnGround, const std::vector<SDL_Rect>& tiles) {
    bool collided = false; // Biến để theo dõi xem có va chạm xảy ra không

    for (const auto& tile : tiles) {
        if (CheckCollision(playerRect, tile)) { 
            ResolveCollision(playerRect, velocityX, velocityY, isOnGround, tile); // Giải quyết va chạm
            collided = true;
        }
    }
    return collided; 
}

bool Collision::CheckCollision(const SDL_Rect& a, const SDL_Rect& b) const {
    // Thuật toán AABB (Axis-Aligned Bounding Box) collision detection
    return (a.x < b.x + b.w &&
        a.x + a.w > b.x &&
        a.y < b.y + b.h &&
        a.y + a.h > b.y);
}

void Collision::ResolveCollision(SDL_Rect& playerRect, float& velocityX, float& velocityY, bool& isOnGround, const SDL_Rect& tile) {
    // Tính toán độ chồng lấp (overlap) giữa player và tile theo trục X và Y
    float overlapX = std::min(
        playerRect.x + playerRect.w - tile.x, // Khoảng cách từ cạnh phải player đến cạnh trái tile
        tile.x + tile.w - playerRect.x          // Khoảng cách từ cạnh phải tile đến cạnh trái player
    );
    float overlapY = std::min(
        playerRect.y + playerRect.h - tile.y, // Khoảng cách từ cạnh dưới player đến cạnh trên tile
        tile.y + tile.h - playerRect.y          // Khoảng cách từ cạnh dưới tile đến cạnh trên player
    );

    // Va chạm xảy ra theo trục X
    if (overlapX < overlapY) {
        velocityX = 0; 
        // Di chuyển player ra khỏi tile
        playerRect.x = (playerRect.x < tile.x) ? tile.x - playerRect.w : tile.x + tile.w;
    }
    else {
        // Kiểm tra xem player có đang ở trên tile không
        if (playerRect.y + playerRect.h <= tile.y + (tile.h / 2)) {
            isOnGround = true; 
            velocityY = 0;   
            playerRect.y = tile.y - playerRect.h; // Đặt player lên trên tile
        }
        // Nếu không, player đang va chạm với cạnh dưới của tile
        else {
            velocityY = 0;     
            playerRect.y = tile.y + tile.h; // Đặt player xuống dưới tile
            isOnGround = false;    
        }
    }
}