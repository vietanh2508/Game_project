#ifndef LEVEL_H
#define LEVEL_H

#include <SDL.h>
#include <string>
#include <vector>
#include <map>

class Renderer;

struct Tile {
    SDL_Texture* texture = nullptr;
    SDL_Rect rect;
};

class Level {
public:
    Level(Renderer& renderer, const std::string& mapPath, const std::string& tileBasePath, int tileWidth, int tileHeight, int mapWidth, int mapHeight);
    ~Level();

    void Render(Renderer& renderer);
    const std::vector<Tile>& GetTiles() const { return tiles; }

private:
    bool LoadLevel(Renderer& renderer, const std::string& mapPath, const std::string& tileBasePath, int tileWidth, int tileHeight);

    std::vector<Tile> tiles;
    int mapWidth;
    int mapHeight;
    int tileWidth;
    int tileHeight;
    std::map<int, SDL_Texture*> tileTextures;
};

#endif