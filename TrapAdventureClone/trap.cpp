#include "trap.h"
#include "collision.h"
#include <cmath>

Trap::Trap(int x, int y, int w, int h, SDL_Texture* texture, Behavior behavior)
    : rect{ x, y, w, h }, texture(texture), behavior(behavior) , initialX(x) {}

SDL_Rect Trap::GetRect() const {
    return rect;
}

SDL_Texture* Trap::GetTexture() const {
    return texture;
}

void Trap::Update(float deltaTime, const SDL_Rect& playerRect, const std::vector<SDL_Rect>& walls) {
    if (!isActivated) {
        float dx = static_cast<float>(playerRect.x - rect.x);
        float dy = static_cast<float>(playerRect.y - rect.y);
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance < 96.0f) {
            isActivated = true;
        }
        return;
    }
   
    if (hasHitWall) return;

    switch (behavior) {
    case Behavior::SHOOT_LEFT: {
        rect.x -= static_cast<int>(slideSpeed * deltaTime);
        for (const auto& wall : walls) {
            if (collision.CheckCollision(rect, wall)) {
                hasHitWall = true;
                break;
            }
        }
        break;
    }
    case Behavior::SHOOT_RIGHT: {
        rect.x += static_cast<int>(slideSpeed * deltaTime);
        for (const auto& wall : walls) {
            if (collision.CheckCollision(rect, wall)) {
                hasHitWall = true;
                break;
            }
        }
        break;
    }
    case Behavior::SLIDE_RIGHT: {
        if (rect.x < initialX + 32) {
            rect.x += static_cast<int>(slideSpeed * deltaTime);
            if (rect.x >= initialX + 32) {
                rect.x = initialX + 32; 
                hasHitWall = true; 
            }
        }
        break;
    }
    case Behavior::SLIDE_LEFT: {
        if (rect.x > initialX - 32) {
            rect.x -= static_cast<int>(slideSpeed * deltaTime);
            if (rect.x <= initialX - 32) {
                rect.x = initialX - 32; 
                hasHitWall = true; 
            }
        }
        break;
    }

    case Behavior::FALL: {
        if (!hasHitWall) {
            rect.y += static_cast<int>(fallSpeed * deltaTime);
            fallSpeed += gravity * deltaTime;

            for (const auto& wall : walls) {
                if (collision.CheckCollision(rect, wall)) {
                    hasHitWall = true;
                    fallSpeed = 0;
                    rect.y = wall.y - rect.h;
                    break;
                }
            }
        }
        break;
    }

    default: 
        break;
    }
}
