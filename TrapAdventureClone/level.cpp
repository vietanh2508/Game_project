#include "level.h"

Level::Level() {
    for (int i = 0; i < 800; i += 32) {
        groundTiles.push_back({ i, 500, 32, 32 });
    }
}

const std::vector<SDL_Rect>& Level::GetGroundTiles() const {
    return groundTiles;
}