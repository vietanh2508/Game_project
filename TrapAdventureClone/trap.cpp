#include "trap.h"

Trap::Trap(int x, int y, int w, int h, SDL_Texture* texture) {
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    this->texture = texture;
}

SDL_Rect Trap::GetRect() const {
    return rect;
}

SDL_Texture* Trap::GetTexture() const {
    return texture;
}