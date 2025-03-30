#ifndef RENDERER_H
#define RENDERER_H

#include<SDL.h>  
#include<string> 

class Renderer {
public:
    Renderer(); // Constructor
    ~Renderer(); // Destructor

    bool init(SDL_Window* window); // Khởi tạo renderer (liên kết với cửa sổ game)
    void Clear(); // Xóa màn hình (đặt màu nền)
    void Present(); // Hiển thị những gì đã vẽ lên màn hình

    SDL_Texture* LoadTexture(const std::string& path); // Tải ảnh từ file và tạo texture
    void RenderTexture(SDL_Texture* texture, const SDL_Rect& srcRect, const SDL_Rect& destRect, bool flip = false); // Vẽ texture lên màn hình

    SDL_Renderer* GetSDLRenderer() const { return renderer; } // Truy cập trực tiếp vào SDL_Renderer 

private:
    SDL_Renderer* renderer; // Con trỏ đến SDL_Renderer 
};

#endif