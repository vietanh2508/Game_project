#include "collision.h"

bool Collision::HandleCollisions(SDL_Rect& playerRect, float& velocityX, float& velocityY, bool& isOnGround, const std::vector<SDL_Rect>& tiles) {
    bool collided = false;
    isOnGround = false;

    for (const auto& tile : tiles) {
        if (CheckCollision(playerRect, tile)) {
            ResolveCollision(playerRect, velocityX, velocityY, isOnGround, tile);
            collided = true;
        }
    }
    return collided;
}

bool Collision::CheckCollision(const SDL_Rect& a, const SDL_Rect& b) const {
    return (a.x < b.x + b.w &&
        a.x + a.w > b.x &&
        a.y < b.y + b.h &&
        a.y + a.h > b.y);
}

void Collision::ResolveCollision(SDL_Rect& playerRect, float& velocityX, float& velocityY, bool& isOnGround, const SDL_Rect& tile) {
    // Tính toán overlap trên từng trục
    float overlapX = (playerRect.x + playerRect.w - tile.x);
    float overlapX2 = (tile.x + tile.w - playerRect.x);
    float minOverlapX = std::min(overlapX, overlapX2);

    float overlapY = (playerRect.y + playerRect.h - tile.y);
    float overlapY2 = (tile.y + tile.h - playerRect.y);
    float minOverlapY = std::min(overlapY, overlapY2);

    // Xác định hướng va chạm dựa trên overlap nhỏ nhất
    if (minOverlapX < minOverlapY) {
        // Va chạm ngang
        if (playerRect.x < tile.x) {
            playerRect.x = tile.x - playerRect.w;
        }
        else {
            playerRect.x = tile.x + tile.w;
        }
        velocityX = 0;
    }
    else {
        // Va chạm dọc
        if (playerRect.y < tile.y) {
            playerRect.y = tile.y - playerRect.h;
            isOnGround = true; // Chỉ set isOnGround khi chạm từ trên xuống
        }
        else {
            playerRect.y = tile.y + tile.h;
        }
        velocityY = 0;
    }
}