#include "level.h"
#include <SDL_image.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include "renderer.h"

// Constructor: Gọi LoadLevel để tải level từ file
Level::Level(
    Renderer& renderer,
    const std::string& mapPath,
    const std::string& tileBasePath,
    const std::string& trapPath,
    const std::string& trapTexturePath,
    int tileWidth, int tileHeight,
    int mapWidth, int mapHeight
) : tileWidth(tileWidth), tileHeight(tileHeight), mapWidth(mapWidth), mapHeight(mapHeight) {
    if (!LoadLevel(renderer, mapPath, tileBasePath, trapPath, trapTexturePath, tileWidth, tileHeight)) {
        std::cerr << "Failed to load level!" << std::endl;
    }
}

// Destructor: Giải phóng bộ nhớ
Level::~Level() {
    for (auto& it : tileTextures) {
        if (it.second) {
            SDL_DestroyTexture(it.second);
        }
    }
}

// LoadLevel: Tải level từ file
bool Level::LoadLevel(
    Renderer& renderer,
    const std::string& mapPath,
    const std::string& tileBasePath,
    const std::string& trapPath,
    const std::string& trapTexturePath,
    int tileWidth, int tileHeight
) {
    // 1. Đọc dữ liệu map từ file CSV
    std::ifstream file(mapPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open map file: " << mapPath << std::endl;
        return false;
    }

    // Đọc từng dòng từ file
    std::vector<std::vector<int>> mapData;
    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;// Lưu trữ tile ID của từng ô trong dòng
        std::stringstream ss(line);
        std::string tileIdStr;  // Lưu trữ tile ID dưới dạng chuỗi

        while (std::getline(ss, tileIdStr, ',')) {
            tileIdStr.erase(std::remove_if(tileIdStr.begin(), tileIdStr.end(), ::isspace), tileIdStr.end());// Xóa khoảng trắng
            try {
                row.push_back(std::stoi(tileIdStr));// Chuyển chuỗi thành số nguyên và thêm vào vector
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Invalid tile ID: " << tileIdStr << std::endl;
                file.close();
                return false;
            }
        }
        mapData.push_back(row); // Thêm dòng vào dữ liệu map    
    }
    file.close();

    size_t actualMapHeight = mapData.size();
    size_t actualMapWidth = (actualMapHeight > 0) ? mapData[0].size() : 0;

    std::set<int> uniqueTileIds; // Sử dụng set để lưu trữ các tile ID duy nhấ
    for (const auto& row : mapData) {
        for (int tileId : row) {
            if (tileId != -1) {
                uniqueTileIds.insert(tileId);
            }
        }
    }

    for (int tileId : uniqueTileIds) {
        std::string tilePath = tileBasePath + std::to_string(tileId) + ".png"; // Tạo đường dẫn đến file texture
        SDL_Texture* texture = renderer.LoadTexture(tilePath); // Tải texture
        if (!texture) {
            std::cerr << "Failed to load tile texture: " << tilePath << std::endl;
        }
        tileTextures[tileId] = texture;// Lưu texture vào map
    }

    for (size_t y = 0; y < actualMapHeight; ++y) {
        for (size_t x = 0; x < actualMapWidth; ++x) {
            int tileId = mapData[y][x];// Lấy tile ID từ dữ liệu map
            if (tileId == -1) continue;

            Tile newTile; // Tạo một tile mới
            if (tileTextures.find(tileId) != tileTextures.end()) { // Kiểm tra xem texture của tile đã được tải chưa
                newTile.texture = tileTextures[tileId]; // Gán texture cho tile
            }

            newTile.rect.x = static_cast<int>(x) * tileWidth; // Tính toán vị trí X của tile
            newTile.rect.y = static_cast<int>(y) * tileHeight; // Tính toán vị trí Y của tile
            newTile.rect.w = tileWidth;   // Gán kích thước cho tile
            newTile.rect.h = tileHeight;   // Gán kích thước cho tile
            tiles.push_back(newTile); // Thêm tile vào danh sách các tile
        }
    }

    std::map<int, SDL_Texture*> trapTextures;
    // Load các texture tương ứng với từng loại bẫy
    trapTextures[0] = renderer.LoadTexture(trapTexturePath + "0.png");
    trapTextures[2] = renderer.LoadTexture(trapTexturePath + "2.png");
    trapTextures[3] = renderer.LoadTexture(trapTexturePath + "2.png");
    trapTextures[8] = renderer.LoadTexture(trapTexturePath + "8.png");
    trapTextures[10] = renderer.LoadTexture(trapTexturePath + "10.png");
    trapTextures[11] = renderer.LoadTexture(trapTexturePath + "10.png");

    std::ifstream trapFile(trapPath);
    if (!trapFile.is_open()) {
        std::cerr << "Failed to open trap file: " << trapPath << std::endl;
        return false;
    }

    std::string lineT;
    int y = 0;
    // Đọc từng dòng của file
    while (std::getline(trapFile, lineT)) {
        std::stringstream ss(lineT);
        std::string cell;
        int x = 0;
        // Đọc từng giá trị trong dòng, phân tách bởi dấu phẩy
        while (std::getline(ss, cell, ',')) {
            int cellValue = std::stoi(cell);
            // Kiểm tra xem giá trị ô có phải là loại bẫy hợp lệ hay không
            if (cellValue == 0 || cellValue == 2 || cellValue == 3 || cellValue == 8 || cellValue == 10 || cellValue == 11) {
                SDL_Texture* texture = trapTextures[cellValue];// Lấy texture tương ứng
                if (texture) {
                    int trapWidth, trapHeight;
                    int adjustedX = x * tileWidth; // Tính toán vị trí x dựa trên ô
                    int adjustedY = y * tileHeight; // Tính toán vị trí y dựa trên ô
                    Trap::Behavior behavior;

                    // Xác định thuộc tính của từng loại bẫy
                    switch (cellValue) {
                    case 0:
                        trapWidth = 16;
                        trapHeight = 32;
                        behavior = Trap::Behavior::SHOOT_LEFT;
                        adjustedX += tileWidth - trapWidth;
                        break;
                    case 2:
                        trapWidth = 32;
                        trapHeight = 16;
                        behavior = Trap::Behavior::SLIDE_RIGHT;
                        adjustedY += tileHeight - trapHeight;
                        break;
                    case 3:
                        trapWidth = 32;
                        trapHeight = 16;
                        behavior = Trap::Behavior::SLIDE_LEFT;
                        adjustedY += tileHeight - trapHeight;
                        break;
                    case 8:
                        trapWidth = 16;
                        trapHeight = 32;
                        behavior = Trap::Behavior::SHOOT_RIGHT;
                        break;
                    case 10:
                        trapWidth = 32;
                        trapHeight = 16;
                        behavior = Trap::Behavior::FALL;
                        break;
                    case 11:
                        trapWidth = 32;
                        trapHeight = 16;
                        behavior = Trap::Behavior::STATIC;
                        break;
                    default:
                        trapWidth = 0;
                        trapHeight = 0;
                        behavior = Trap::Behavior::STATIC;
                        break;
                    }

                    // Thêm bẫy vào danh sách với các thuộc tính đã thiết lập
                    traps.emplace_back(
                        adjustedX,
                        adjustedY,
                        trapWidth,
                        trapHeight,
                        texture,
                        behavior
                    );
                }
            }
            x++; // Tăng vị trí x cho ô tiếp theo
        }
        y++; // Tăng vị trí y cho dòng tiếp theo
    }
    trapFile.close();

    return true;
}

// Render: Vẽ level lên màn hình
void Level::Render(Renderer& renderer) {
    for (const auto& tile : tiles) {
        if (tile.texture) {
            SDL_Rect srcRect = { 0, 0, tileWidth, tileHeight }; // Lấy toàn bộ texture
            renderer.RenderTexture(tile.texture, srcRect, tile.rect); // Vẽ tile
        }
    }

    for (const auto& trap : traps) {
        SDL_Rect srcRect = { 0, 0, trap.GetRect().w, trap.GetRect().h }; // Lấy toàn bộ texture của trap
        renderer.RenderTexture(trap.GetTexture(), srcRect, trap.GetRect()); // Vẽ trap
    }
}

const std::vector<Trap>& Level::GetTraps() const {
    return traps;
}

void Level::Update(float deltaTime, const SDL_Rect& playerRect, const std::vector<SDL_Rect>& walls) {
    for (auto& trap : traps) {
        trap.Update(deltaTime, playerRect, walls);
    }
}

void Level::ResetTraps() {
    for (auto& trap : traps) {
        trap.Reset();
    }
}