#ifndef COLLISION_H
#define COLLISION_H

#include <SDL.h>
#include <vector>

class Collision {
public:
    bool HandleCollisions(SDL_Rect& playerRect, float& velocityX, float& velocityY, bool& isOnGround, const std::vector<SDL_Rect>& tiles);
    bool CheckCollision(const SDL_Rect& a, const SDL_Rect& b) const;
private:
    void ResolveCollision(SDL_Rect& playerRect, float& velocityX, float& velocityY, bool& isOnGround, const SDL_Rect& tile);
};

#endif