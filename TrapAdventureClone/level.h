#ifndef LEVEL_H
#define LEVEL_H

#include <SDL.h>
#include <vector>

class Level {
public:
    Level();
    const std::vector<SDL_Rect>& GetGroundTiles() const;

private:
    std::vector<SDL_Rect> groundTiles;
};

#endif 