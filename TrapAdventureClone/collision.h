#ifndef COLLISION_H
#define COLLISION_H

#include <SDL.h>     
#include <vector>    

class Collision {
public:
    // Hàm chính để xử lý va chạm giữa player và các tile
    bool HandleCollisions(SDL_Rect& playerRect, float& velocityX, float& velocityY, bool& isOnGround, const std::vector<SDL_Rect>& tiles);

    // Hàm kiểm tra xem hai hình chữ nhật có va chạm không
    bool CheckCollision(const SDL_Rect& a, const SDL_Rect& b) const;

private:
    // Hàm giải quyết va chạm (di chuyển player ra khỏi tile)
    void ResolveCollision(SDL_Rect& playerRect, float& velocityX, float& velocityY, bool& isOnGround, const SDL_Rect& tile);
};

#endif