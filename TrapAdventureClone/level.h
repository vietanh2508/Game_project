#ifndef LEVEL_H
#define LEVEL_H

#include <SDL.h>
#include <string>
#include <vector>
#include <map>
#include "trap.h"
#include "renderer.h"

// Struct Tile: Đại diện cho một ô gạch trong level
struct Tile {
    SDL_Texture* texture = nullptr;
    SDL_Rect rect;
};

class Level {
public:
    // Constructor: Tải level từ file
    Level(
        Renderer& renderer,               // Tham chiếu đến đối tượng Renderer
        const std::string& mapPath,        // Đường dẫn đến file map (CSV)
        const std::string& tileBasePath,   // Đường dẫn gốc cho các file texture của tile
        const std::string& trapPath,        // Đường dẫn đến file trap (CSV)
        const std::string& trapTexturePath, // Đường dẫn gốc cho các file texture của trap
        int tileWidth, int tileHeight,     // Kích thước của mỗi tile
        int mapWidth, int mapHeight         // Kích thước của map (tính bằng số lượng tile)
    );
    ~Level(); // Destructor: Giải phóng bộ nhớ

    void Render(Renderer& renderer);                               // Vẽ level lên màn hình
    const std::vector<Tile>& GetTiles() const { return tiles; }    // Lấy danh sách các tile
    const std::vector<Trap>& GetTraps() const;                      // Lấy danh sách các trap
    void Update(float deltaTime, const SDL_Rect& playerRect, const std::vector<SDL_Rect>& walls); // Cập nhật trạng thái của level (ví dụ: cập nhật trạng thái của các trap)
    bool LoadLevel(                                                 // Tải level từ file (hàm này được gọi bởi constructor)
        Renderer& renderer,
        const std::string& mapPath,
        const std::string& tileBasePath,
        const std::string& trapPath,
        const std::string& trapTexturePath,
        int tileWidth, int tileHeight
    );
    void ResetTraps();                                             // Reset trạng thái của các trap

private:
    std::vector<Trap> traps;                            // Danh sách các trap trong level
    std::vector<Tile> tiles;                           // Danh sách các tile trong level
    int mapWidth;                                       // Kích thước của map (tính bằng số lượng tile)
    int mapHeight;                                      // Kích thước của map (tính bằng số lượng tile)
    int tileWidth;                                      // Kích thước của mỗi tile
    int tileHeight;                                     // Kích thước của mỗi tile
    std::map<int, SDL_Texture*> tileTextures;          // Lưu trữ các texture của tile (tileId -> texture)
};

#endif