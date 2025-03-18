#include "Level.h"
#include <SDL_image.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include "renderer.h"

Level::Level(Renderer& renderer, const std::string& mapPath, const std::string& tileBasePath, int tileWidth, int tileHeight, int mapWidth, int mapHeight)
    : tileWidth(tileWidth), tileHeight(tileHeight), mapWidth(mapWidth), mapHeight(mapHeight) {
    if (!LoadLevel(renderer, mapPath, tileBasePath, tileWidth, tileHeight)) {
        std::cerr << "Failed to load level!" << std::endl;
    }
}

Level::~Level() {
    for (auto& it : tileTextures) {
        if (it.second) {
            SDL_DestroyTexture(it.second);
        }
    }
}

bool Level::LoadLevel(Renderer& renderer, const std::string& mapPath, const std::string& tileBasePath, int tileWidth, int tileHeight) {
    std::ifstream file(mapPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open map file: " << mapPath << std::endl;
        return false;
    }

    std::vector<std::vector<int>> mapData;
    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        std::stringstream ss(line);
        std::string tileIdStr;

        while (std::getline(ss, tileIdStr, ',')) {
            tileIdStr.erase(std::remove_if(tileIdStr.begin(), tileIdStr.end(), ::isspace), tileIdStr.end());
            try {
                row.push_back(std::stoi(tileIdStr));
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Invalid tile ID: " << tileIdStr << std::endl;
                file.close();
                return false;
            }
        }
        mapData.push_back(row);
    }
    file.close();

    size_t actualMapHeight = mapData.size();
    size_t actualMapWidth = (actualMapHeight > 0) ? mapData[0].size() : 0;

    for (const auto& row : mapData) {
        if (row.size() != actualMapWidth) {
            std::cerr << "Error: Inconsistent column count in CSV file!" << std::endl;
            return false;
        }
    }

    std::set<int> uniqueTileIds;
    for (const auto& row : mapData) {
        for (int tileId : row) {
            if (tileId != -1) {
                uniqueTileIds.insert(tileId);
            }
        }
    }

    tileTextures.clear();
    for (int tileId : uniqueTileIds) {
        std::string tilePath = tileBasePath + std::to_string(tileId) + ".png";
        SDL_Texture* texture = renderer.LoadTexture(tilePath);
        if (!texture) {
            std::cerr << "Failed to load tile texture: " << tilePath << std::endl;
        }
        tileTextures[tileId] = texture;
    }

    tiles.clear();
    for (size_t y = 0; y < actualMapHeight; ++y) {
        for (size_t x = 0; x < actualMapWidth; ++x) {
            int tileId = mapData[y][x];
            if (tileId == -1) continue;

            Tile newTile;
            if (tileTextures.find(tileId) != tileTextures.end()) {
                newTile.texture = tileTextures[tileId];
            }

            newTile.rect.x = static_cast<int>(x) * tileWidth;  
            newTile.rect.y = static_cast<int>(y) * tileHeight; 
            newTile.rect.w = tileWidth;
            newTile.rect.h = tileHeight;
            tiles.push_back(newTile);
        }
    }

    return true;
}

void Level::Render(Renderer& renderer) {
    for (const auto& tile : tiles) {
        if (tile.texture) {
            SDL_Rect srcRect = { 0, 0, tileWidth, tileHeight };
            renderer.RenderTexture(tile.texture, srcRect, tile.rect);
        }
    }
}