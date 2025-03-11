#ifndef LEVEL_H
#define LEVEL_H

#include <SDL.h>
#include <vector>
#include <string>


class Level {
public:
    Level();
    const std::vector<SDL_Rect>& GetGroundTiles() const;
    std::vector<SDL_Rect> GetTilesAsRects() const;
private:
    std::vector<SDL_Rect> groundTiles;
    
};

#endif 