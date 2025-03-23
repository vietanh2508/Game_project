#ifndef TRAP_H
#define TRAP_H

#include <SDL.h>
#include <vector>
#include "collision.h"

class Trap {
public:
    enum class Behavior {
        STATIC,
        SLIDE_RIGHT,
        SLIDE_LEFT,
        FALL,
        SHOOT_LEFT,   
        SHOOT_RIGHT      
    };

    Trap(int x, int y, int w, int h, SDL_Texture* texture, Behavior behavior);

    SDL_Rect GetRect() const;
    SDL_Texture* GetTexture() const;
    void Update(float deltaTime, const SDL_Rect& playerRect, const std::vector<SDL_Rect>& walls);

private:
    SDL_Rect rect;
    SDL_Texture* texture;
    Behavior behavior;
    Collision collision;

    bool isActivated = false;
    bool isFalling = false;
    bool isSlidingLeft = false;
    bool hasHitWall = false;
    bool isSlidingRight = true; 
    float slideSpeed = 200.0f;  
    float fallSpeed = 400.0f;  
    const float gravity = 980.0f;
    int initialX;
};

#endif