#ifndef TRAP_H
#define TRAP_H

#include <SDL.h>

class Trap {
public:
    Trap(int x, int y, int w, int h, SDL_Texture* texture);
    SDL_Rect GetRect() const;      
    SDL_Texture* GetTexture() const; 

private:
    SDL_Rect rect;   
    SDL_Texture* texture; 
};

#endif