#include "level.h"

Level::Level() {
    for (int i = 0; i < 960; i += 32) {
        groundTiles.push_back({ i, 640, 32, 32 });
    }
}

const std::vector<SDL_Rect>& Level::GetGroundTiles() const {
    return groundTiles;
}

std::vector<SDL_Rect> Level::GetTilesAsRects() const {
    return groundTiles; 
}