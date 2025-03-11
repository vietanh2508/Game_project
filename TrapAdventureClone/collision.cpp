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

    float overlapX = std::min(playerRect.x + playerRect.w - tile.x, tile.x + tile.w - playerRect.x);
    float overlapY = std::min(playerRect.y + playerRect.h - tile.y, tile.y + tile.h - playerRect.y);

    if (overlapX < overlapY) {
        if (playerRect.x < tile.x) {
            playerRect.x = tile.x - playerRect.w;
        }
        else {
            playerRect.x = tile.x + tile.w;
        }
        velocityX = 0;
    }
    else {
        if (playerRect.y < tile.y) {
            playerRect.y = tile.y - playerRect.h;
            velocityY = 0;
            isOnGround = true;
        }
        else {
            playerRect.y = tile.y + tile.h;
            velocityY = 0;
        }
    }
}