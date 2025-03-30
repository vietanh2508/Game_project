#include "collision.h"
#include <iostream>

bool Collision::HandleCollisions(SDL_Rect& playerRect, float& velocityX, float& velocityY, bool& isOnGround, const std::vector<SDL_Rect>& tiles) {
    bool collided = false;

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
    float overlapX = std::min(
        static_cast<float>(playerRect.x + playerRect.w - tile.x),  
        static_cast<float>(tile.x + tile.w - playerRect.x)   
    );
    float overlapY = std::min(
        static_cast<float>(playerRect.y + playerRect.h - tile.y), 
        static_cast<float>(tile.y + tile.h - playerRect.y)  
    );

    if (overlapX < overlapY) {
        velocityX = 0;
        playerRect.x = (playerRect.x < tile.x) ? tile.x - playerRect.w : tile.x + tile.w;
    }
    else {
        if (velocityY > 0) {
            isOnGround = true;
            velocityY = 0;
            playerRect.y = tile.y - playerRect.h;
        }
        else if (velocityY < 0) {
            velocityY = 0;
            playerRect.y = tile.y + tile.h;
        }
    }
}